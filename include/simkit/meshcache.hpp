#ifndef __SIMKIT_MESHCACHE_HPP__
#define __SIMKIT_MESHCACHE_HPP__

#include <simkit.h>

namespace SimKit {
    /* Represents a template for a mesh data cache.
     * 
     * Since mesh data doesn't have a fairly standard representation like, say,
     * SDL_Texture, we instead have two template parameters:
     * 
     * gpu_context_type: Type that represents a particular rendering context,
     *  i.e. the range of which a particular context-specific piece of data is
     *  valid.
     * gpu_data_type: Type that represents data that has been uploaded to the
     *  renderer. It should have a destructor which frees any resources
     *  allocated to it.
     * gpu_traits: A class with the following static helper functions:
     *  gpu_data_type* upload_data(gpu_context_type*, float*, MeshDataType, IVMeshData::Usage),
     *  gpu_data_type* upload_data(gpu_context_type*, int*, MeshDataType, IVMeshData::Usage):
     *   Given a float or int array, allocate context-specific data for this
     *   type and return it to us. The MeshDataType indicates if the data is
     *   vertex data, index data, normals, uvs, or attributes. Usage indicates
     *   how frequently the data changes.
     */
    template <typename gpu_context_type, typename gpu_data_type, typename gpu_traits>
    class SIMKIT_API TMeshCache {
    private:
        struct HWCacheData {
            gpu_context_type* context;
            gpu_data_type* vertex_data, index_data, normal_data;
            
            std::map<int, gpu_data_type*> uvmap_data;
            std::map<int, gpu_data_type*> attrib_data;
        };
        
        struct CacheData {
            IVMeshData* virtual_mesh;
            IVMeshData::IRequest* loaded_mesh;
            
            float* vertex_data;
            int* index_data;
            float* normal_data;
            std::map<int, float*> uvmap_data;
            std::map<int, float*> attrib_data;
            
            std::map<gpu_context_type*, HWCacheData> hw;
        };
        
        std::map<IVMeshData*, CacheData> cache;
        
        enum MeshDataType {
            VERTEX_DATA, INDEX_DATA, NORMAL_DATA, UVMAP_DATA, ATTRIB_DATA
        };
        
    protected:
        TMeshCache() {};
    
    public:
        virtual ~TMeshCache() {
            for (std::map<IVMeshData*, CacheData>::iterator i = this->cache.begin(); i != this->cache.end(); i++) {
                if (i->vertex_data) delete[] i->vertex_data;
                if (i->index_data) delete[] i->index_data;
                if (i->normal_data) delete[] i->normal_data;

                for (std::map<int, float*> uvmap_data)
            }
        };

        
    }
}

#endif
