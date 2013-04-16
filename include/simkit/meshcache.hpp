#ifndef __SIMKIT_MESHCACHE_HPP__
#define __SIMKIT_MESHCACHE_HPP__

#include <simkit.h>

namespace SimKit {
    class MDeleteHook;
    
    /* Represents a template for a mesh data cache.
     * 
     * Since mesh data doesn't have a fairly standard representation like, say,
     * SDL_Texture, we instead have two template parameters:
     * 
     * gpu_context_type: Type that represents a particular rendering context,
     *  i.e. the range of which a particular context-specific piece of data is
     *  valid.
     * 
     * gpu_data_type: Type that represents data that has been uploaded to the
     *  GPU context.
     * 
     * gpu_traits: A class with the following static helper functions:
     *  gpu_data_type upload_data(gpu_context_type, float*, int, MeshDataType, IVMeshData::Usage),
     *  gpu_data_type upload_data(gpu_context_type, int*, int, MeshDataType, IVMeshData::Usage),
     *  gpu_data_type upload_data(gpu_context_type, float*, int, MeshDataType, IVMeshData::Usage, gpu_data_type),
     *  gpu_data_type upload_data(gpu_context_type, int*, int, MeshDataType, IVMeshData::Usage, gpu_data_type):
     *   Given a float or int array and it's size, allocate context-specific
     *   data for this type and return it to us. The MeshDataType indicates if
     *   the data is vertex data, index data, normals, uvs, or attributes. Usage
     *   indicates how frequently the data changes.
     *   
     *   The latter two functions update existing gpu_data, destroying the data
     *   on the context side and replacing it with new data. They will return
     *   the new data value (which may or may not be different).
     *  
     *  gpu_data_type destroy_data(gpu_context_type, gpu_data_type),
     *  gpu_data_type null(gpu_context_type):
     *   Two functions that return the null value for the type. A "null value"
     *   exists specifically to indicate that there is no data on the context's
     *   side at all.
     *   
     *   The "destroy_data" variant takes a data value, destroys the data, and
     *   returns the null value. It is legal to call destroy_data with the
     *   context's null value; in this case the function should do nothing but
     *   return.
     */
    template <typename gpu_context_type, typename gpu_data_type, typename gpu_traits>
    class SIMKIT_API TMeshCache : public virtual MDeleteHook::IHandler, public virtual IVMeshData::IRequest::IListener {
    private:
        struct HWCacheData {
            gpu_context_type context;
            gpu_data_type vertex_data, index_data, normal_data;
            
            std::map<int, gpu_data_type> uvmap_data;
            std::map<int, gpu_data_type> attrib_data;
        };

        struct FloatArray {
            float* data;
            int count;
        };
        
        struct CacheData {
            IVMeshData* virtual_mesh;
            IVMeshData::IRequest* loaded_mesh;
            
            float* vertex_data; int vertex_count;
            int* index_data;    int index_count;
            float* normal_data; int normal_count;
            std::map<int, FloatArray> uvmap_data;
            std::map<int, FloatArray> attrib_data;
            
            std::map<gpu_context_type, HWCacheData> hw;
        };
        
        std::map<IVMeshData*, CacheData> cache;
        
        enum MeshDataType {
            VERTEX_DATA, INDEX_DATA, NORMAL_DATA, UVMAP_DATA, ATTRIB_DATA, NULL_DATA
        };

        void ensure_request_exists(IVMeshData* vmesh, const float desired_quality) {
            if (this->cache.count(vmesh) == 0) {
                this->add_mesh_to_cache(vmesh);
            }
            
            if (!this->cache[vmesh].loaded_mesh) {
                this->cache[vmesh].loaded_mesh = vmesh->get_mesh_data(desired_quality, this);
            }
        };
        
        void ensure_context_exists(IVMeshData* vmesh, gpu_context_type ctxt) {
            //Assumes request exists (do not call before ensure_request_exists)
            if (this->cache[vmesh].hw.count(ctxt) == 0) {
                std::pair<gpu_context_type, HWCacheData> hcd;

                hcd.first = ctxt;
                hcd.second.context = ctxt;
                hcd.second.vertex_data = gpu_traits::null(ctxt);
                hcd.second.index_data = gpu_traits::null(ctxt);
                hcd.second.normal_data = gpu_traits::null(ctxt);
                
                this->cache[vmesh].hw.insert(hcd);
            }
        };

        void ensure_attribute_loaded(IVMeshData* vmesh, const int attrib_id) {
            //Assumes request exists, has completed, and attribute exists
            if (this->cache[vmesh].attrib_data.count(attrib_id) == 0) {
                std::pair<int, FloatArray> attrib_pair;
                attrib_pair.first = attrib_id;
                this->cache[vmesh].loaded_mesh->get_attrib(attrib_id, &attrib_pair.second.data, &attrib_pair.second.count);
                this->cache[vmesh].attrib_data.insert(attrib_pair);
            }
        };

        void ensure_uvmap_loaded(IVMeshData* vmesh, const int uvmap_id) {
            //Assumes request exists, has completed, and uvmap exists
            if (this->cache[vmesh].uvmap_data.count(uvmap_id) == 0) {
                std::pair<int, FloatArray> uvmap_pair;
                uvmap_pair.first = uvmap_id;
                this->cache[vmesh].loaded_mesh->get_uvmap(uvmap_id, &uvmap_pair.second.data, &uvmap_pair.second.count);
                this->cache[vmesh].uvmap_data.insert(uvmap_pair);
            }
        };
    protected:
        TMeshCache() {};
    public:
        virtual ~TMeshCache() {
            while (this->cache.size > 0) {
                invalidate_mesh(this->cache->begin()->first);
            }
        };
        
        virtual void on_delete(MDeleteHook* killed) {
            IVMeshData* deadman = dynamic_cast<IVMeshData*>(killed);

            if (deadman && this->cache.count(deadman) > 0) {
                this->invalidate_mesh(deadman);
            }
        };
        
        virtual void on_request_progress(IVMeshData* vmesh, IVMeshData::IRequest* request, const IVMeshData::IRequest::RequestStatus update) {
            switch (update) {
                case SimKit::IVMeshData::IRequest::REQUEST_INVALID:
                {
                    std::string err = "Mesh request was rejected as invalid (REQUEST_INVALID).\nReason: ";
                    err += request->get_loading_error();
                    SimKit::EmergencyError(err);
                }
                    break;
                case SimKit::IVMeshData::IRequest::REQUEST_FAILED:
                {
                    std::string err = "Mesh source failed to load (REQUEST_FAILED).\nReason: ";
                    err += request->get_loading_error();
                    SimKit::EmergencyError(err);
                }
                    break;
                case SimKit::IVMeshData::IRequest::REQUEST_COMPLETED:
                {
                    request->get_verticies(&this->cache[vmesh].vertex_data, &this->cache[vmesh].vertex_count);
                    request->get_indicies(&this->cache[vmesh].index_data, &this->cache[vmesh].index_count);
                    if (request->has_normals()) request->get_normals(&this->cache[vmesh].normal_data, &this->cache[vmesh].normal_count);
                }
                    break;
                default:
                    break;
            }
        };
        
        void add_mesh_to_cache(IVMeshData* mesh) {
            mesh->register_delete_handler(this);
            
            std::pair<IVMeshData*, CacheData> cd;

            cd.first = mesh;
            cd.second.virtual_mesh = mesh;
            cd.second.loaded_mesh = NULL;
            cd.second.vertex_data = NULL;
            cd.second.index_data = NULL;
            cd.second.normal_data = NULL;
            
            this->cache.insert(cd);
        };

        /* *** VERTEX DATA *** */
        
        IVMeshData::RequestStatus request_mesh_verticies(IVMeshData* vmesh, const float desired_quality, float** out_vertex_data, int* out_vertex_count) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].vertex_data) {
                    if (out_vertex_data) *out_vertex_data = this->cache[vmesh].vertex_data;
                    if (out_vertex_count) *out_vertex_count = this->cache[vmesh].vertex_count;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        IVMeshData::RequestStatus request_mesh_verticies(IVMeshData* vmesh, const float desired_quality, gpu_context_type ctxt, gpu_data_type* out_data) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->ensure_context_exists(vmesh, ctxt);
                
                if (this->cache[vmesh].hw[ctxt].vertex_data != gpu_traits::null(ctxt)) {
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].vertex_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].vertex_data) {
                    this->cache[vmesh].hw[ctxt].vertex_data = gpu_traits::upload_data(ctxt, this->cache[vmesh].vertex_data, this->cache[vmesh].vertex_count, VERTEX_DATA, vmesh->get_usage_frequency());
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].vertex_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        /* *** INDEX DATA *** */
        
        IVMeshData::RequestStatus request_mesh_indicies(IVMeshData* vmesh, const float desired_quality, int** out_index_data, int* out_index_count) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].index_data) {
                    if (out_index_data) *out_index_data = this->cache[vmesh].index_data;
                    if (out_index_count) *out_index_count = this->cache[vmesh].index_count;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no index data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        IVMeshData::RequestStatus request_mesh_indicies(IVMeshData* vmesh, const float desired_quality, gpu_context_type ctxt, gpu_data_type* out_data) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->ensure_context_exists(vmesh, ctxt);
                
                if (this->cache[vmesh].hw[ctxt].index_data != gpu_traits::null(ctxt)) {
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].index_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].index_data) {
                    this->cache[vmesh].hw[ctxt].index_data = gpu_traits::upload_data(ctxt, this->cache[vmesh].index_data, this->cache[vmesh].index_count, INDEX_DATA, vmesh->get_usage_frequency());
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].index_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no index data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };

        /* *** NORMAL DATA *** */
        
        IVMeshData::RequestStatus request_mesh_normals(IVMeshData* vmesh, const float desired_quality, float** out_normal_data, int* out_normal_count, bool* out_has_normals) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }

                if (!this->cache[vmesh].loaded_mesh->has_normals()) {
                    if (out_has_normals) *out_has_normals = this->cache[vmesh].loaded_mesh->has_normals();
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].normal_data) {
                    if (out_has_normals) *out_has_normals = this->cache[vmesh].loaded_mesh->has_normals();
                    if (out_normal_data) *out_normal_data = this->cache[vmesh].normal_data;
                    if (out_normal_count) *out_normal_count = this->cache[vmesh].normal_count;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        IVMeshData::RequestStatus request_mesh_normals(IVMeshData* vmesh, const float desired_quality, gpu_context_type ctxt, gpu_data_type* out_data, bool* out_has_normals) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (!this->cache[vmesh].loaded_mesh->has_normals()) {
                    if (out_has_normals) *out_has_normals = this->cache[vmesh].loaded_mesh->has_normals();
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->ensure_context_exists(vmesh, ctxt);
                
                if (this->cache[vmesh].hw[ctxt].normal_data != gpu_traits::null(ctxt)) {
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].normal_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].normal_data) {
                    this->cache[vmesh].hw[ctxt].normal_data = gpu_traits::upload_data(ctxt, this->cache[vmesh].normal_data, this->cache[vmesh].normal_count, NORMAL_DATA, vmesh->get_usage_frequency());
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].normal_data;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };

        /* *** GENERIC VERTEX ATTRIBUTE DATA *** */
        
        IVMeshData::RequestStatus request_mesh_attribs(IVMeshData* vmesh, const float desired_quality, const int attrib_id, float** out_attrib_data, int* out_attrib_count, bool* out_has_attrib) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }

                if (!this->cache[vmesh].loaded_mesh->has_attrib(attrib_id)) {
                    if (out_has_attrib) *out_has_attrib = this->cache[vmesh].loaded_mesh->has_attrib(attrib_id);
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }

                this->ensure_attribute_loaded(vmesh, attrib_id);
                
                if (this->cache[vmesh].attrib_data[attrib_id].data) {
                    if (out_attrib_data) *out_attrib_data = this->cache[vmesh].attrib_data[attrib_id].data;
                    if (out_attrib_count) *out_attrib_count = this->cache[vmesh].attrib_data[attrib_id].count;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        IVMeshData::RequestStatus request_mesh_attribs(IVMeshData* vmesh, const float desired_quality, const int attrib_id, gpu_context_type ctxt, gpu_data_type* out_data, bool* out_has_attrib) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (!this->cache[vmesh].loaded_mesh->has_attrib(attrib_id)) {
                    if (out_has_attrib) *out_has_attrib = this->cache[vmesh].loaded_mesh->has_attrib(attrib_id);
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->ensure_context_exists(vmesh, ctxt);
                this->ensure_attribute_loaded(vmesh, attrib_id);
                
                if (this->cache[vmesh].hw[ctxt].attrib_data.count(attrib_id) > 0 && this->cache[vmesh].hw[ctxt].attrib_data[attrib_id] != gpu_traits::null(ctxt)) {
                    if (out_has_attrib) *out_has_attrib = this->cache[vmesh].loaded_mesh->has_attrib(attrib_id);
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].attrib_data[attrib_id];
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].attrib_data[attrib_id].data) {
                    this->cache[vmesh].hw[ctxt].attrib_data[attrib_id] = gpu_traits::upload_data(ctxt, this->cache[vmesh].attrib_data[attrib_id].data, this->cache[vmesh].attrib_data[attrib_id].count, ATTRIB_DATA, vmesh->get_usage_frequency());
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].attrib_data[attrib_id];
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };

        /* *** VERTEX IMAGE COORDINATE UV MAPPING DATA *** */
        
        IVMeshData::RequestStatus request_mesh_uvmaps(IVMeshData* vmesh, const float desired_quality, const int uvmap_id, float** out_uvmap_data, int* out_uvmap_count, bool* out_has_uvmap) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }

                if (!this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id)) {
                    if (out_has_uvmap) *out_has_uvmap = this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id);
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }

                this->ensure_uvmap_loaded(vmesh, uvmap_id);
                
                if (this->cache[vmesh].uvmap_data[uvmap_id].data) {
                    if (out_has_uvmap) *out_has_uvmap = this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id);
                    if (out_uvmap_data) *out_uvmap_data = this->cache[vmesh].uvmap_data[uvmap_id].data;
                    if (out_uvmap_count) *out_uvmap_count = this->cache[vmesh].uvmap_data[uvmap_id].count;
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        IVMeshData::RequestStatus request_mesh_uvmaps(IVMeshData* vmesh, const float desired_quality, const int uvmap_id, gpu_context_type ctxt, gpu_data_type* out_data, bool* out_has_uvmap) {
            int reload = 0;
            
            while (reload < 2) {
                reload++;
                
                this->ensure_request_exists(vmesh, desired_quality);
                
                if (this->cache[vmesh].loaded_mesh->check_request_status() != IVMeshData::IRequest::REQUEST_COMPLETE) {
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (!this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id)) {
                    if (out_has_uvmap) *out_has_uvmap = this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id);
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->ensure_context_exists(vmesh, ctxt);
                this->ensure_uvmap_loaded(vmesh, uvmap_id);
                
                if (this->cache[vmesh].hw[ctxt].uvmap_data.count(uvmap_id) > 0 && this->cache[vmesh].hw[ctxt].uvmap_data[uvmap_id] != gpu_traits::null(ctxt)) {
                    if (out_has_uvmap) *out_has_uvmap = this->cache[vmesh].loaded_mesh->has_uvmap(uvmap_id);
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].uvmap_data[uvmap_id];
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                if (this->cache[vmesh].uvmap_data[uvmap_id].data) {
                    this->cache[vmesh].hw[ctxt].uvmap_data[uvmap_id] = gpu_traits::upload_data(ctxt, this->cache[vmesh].uvmap_data[uvmap_id].data, this->cache[vmesh].uvmap_data[uvmap_id].count, UVMAP_DATA, vmesh->get_usage_frequency());
                    if (out_data) *out_data = this->cache[vmesh].hw[ctxt].uvmap_data[uvmap_id];
                    return this->cache[vmesh].loaded_mesh->check_request_status();
                }
                
                this->invalidate_mesh(vmesh);
            }
            
            std::string err = "Mesh source provided no vertex data after explicit reload and REQUEST_COMPLETE status.\n Reason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        };
        
        void invalidate_mesh(IVMeshData* mesh) {
            if (this->cache[mesh].vertex_data) delete[] this->cache[mesh].vertex_data;
            if (this->cache[mesh].index_data) delete[] this->cache[mesh].index_data;
            if (this->cache[mesh].normal_data) delete[] this->cache[mesh].normal_data;
            
            for (std::map<int, FloatArray>::iterator j = this->cache[mesh].uvmap_data.begin(); j != this->cache[mesh].uvmap_data.end(); j++) {
                if (j->second.data) delete[] j->second.data;
            }
            
            for (std::map<int, FloatArray>::iterator j = this->cache[mesh].attrib_data.begin(); j != this->cache[mesh].attrib_data.end(); j++) {
                if (j->second.data) delete[] j->second.data;
            }
            
            for (std::map<gpu_context_type, HWCacheData>::iterator j = this->cache[mesh].hw.begin(); j != this->cache[mesh].hw.end(); j++) {
                gpu_traits::destroy_data(j->first, j->second.vertex_data);
                gpu_traits::destroy_data(j->first, j->second.index_data);
                gpu_traits::destroy_data(j->first, j->second.normal_data);
                
                for (std::map<int, gpu_data_type>::iterator k = j->second.uvmap_data.begin(); k != j->second.uvmap_data.end(); k++) {
                    gpu_traits::destroy_data(j->first, k->second)
                }
                
                for (std::map<int, gpu_data_type>::iterator k = j->second.attrib_data.begin(); k != j->second.attrib_data.end(); k++) {
                    gpu_traits::destroy_data(j->first, k->second)
                }
            }
            
            if (this->cache[mesh].loaded_mesh) delete[] this->cache[mesh].loaded_mesh;
            
            this->cache.erase(mesh);
        };

        void invalidate_context
    }
}

#endif
