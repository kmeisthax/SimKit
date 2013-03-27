#ifndef __SIMKIT_VMESHDATA_H__
#define __SIMKIT_VMESHDATA_H__

#include <simkit.h>
#include <SDL.h>

namespace SimKit {
    /* A VMeshData is a provider of triangular 3D shape data.
     */
    class SIMKIT_API IVMeshData : public virtual ITVResource<IVMeshData> {
    protected:
        IVMeshData();
    public:
        virtual ~IVMeshData() = 0;
        
        /* Mesh data returned by a VMeshData provider may be static, dynamic, or
         * streaming. These correspond directly to the OpenGL buffer object
         * usage classes; furthermore, mesh data caches should take this data
         * into account when choosing to cache mesh data.
         * 
         * By default this function returns STATIC always.
         */
        enum Usage {
            STATIC,
            DYNAMIC,
            STREAM
        };
        
        virtual Usage get_usage_frequency();
        
        /* Mesh data retrieved can exist in multiple quality settings.
         * 
         * Quality is, by convention, specified as minimum feature size in mesh
         * units. For example, if the mesh units are millimeters, and a mesh
         * has it's smallest edge as 1.2mm, then the quality of that mesh is 1.2.
         * 
         * Mesh quality may not exist on a continuous scale. The virtual mesh
         * provider is responsible for selecting an appropriate discrete quality
         * parameter if this is the case. The granularity parameter specifies
         * the average interval between quality parameters; it should be set to
         * zero for continuous mesh quality spaces.
         * 
         * Even though it may be possible for a mesh data provider to provide
         * data outside of it's quality parameters (i.e. if the mesh data is
         * being provided by, say, some kind of parametric or implicit surface)
         * the quality parameter range should be constrained to what constitutes
         * a reasonable range. I.e. minquality should be specified at the
         * smallest quality that will display any sort of triangular geometry;
         * while maxquality should be specified at the maximum quality of which
         * any further increase in quality would not result in an increase of
         * detail.
         */
        virtual void get_quality_range(float* out_minquality, float* out_maxquality, float* out_granularity) = 0;
        
        class SIMKIT_API IRequest : public virtual ITVResource<IVMeshData>::IRequest {
        public:
            typedef ITVResource<IVMeshData>::IRequest::IListener IListener;
        protected:
            IRequest(IVMeshData* parent);
            IRequest(IVMeshData* parent, IListener* listen);
        public:
            /* Retrieve information about how the mesh data should be interpreted during rendering.
             * 
             * Data can be provided as a list of disconnected triangles, a strip
             * of connected triangles, or a fan of connected triangles.
             * 
             * All mesh data is interpreted as indexed rendering with a
             * primitive restart value equal to the number of verticies in the
             * mesh.
             */
            enum PrimType {
                TRIANGLES,
                TRIANGLE_STRIPS,
                TRIANGLE_FANS,
            };
            
            virtual PrimType get_primitive_type() = 0;
            
            /* Retrieve vertex data as a list of (x,y,z) coordinate pairs.
             * 
             * NOTE: All arrays returned by these functions are the property of
             * the first person to retrieve them. These functions may or may not
             * return the same array every time it is used.
             * 
             * The arrays must be allocated with new[], not malloc, as they are
             * to be freed with delete[], not free.
             */
            virtual void get_vertexes(float** out_data, int* out_numfloats) = 0;
            
            /* Retrieve index data as a list of integers.
             * 
             * The vertex index data selects verticies to be assembled into the
             * primitive type. For primitives which have long-running state, one
             * may reset the current primitive and starting over by issuing an
             * index equal to that of the number of verticies. For example,
             * given 10 verticies, one could encode two triangle strips as such:
             * 
             *   indicies = [0, 1, 2, 3, 4, 10, 5, 6, 7, 8, 9]
             */
            virtual void get_indicies(int** out_data, int* out_numints) = 0;
            
            /* Retrieve ancillary non-shape information useful for materials.
             * 
             * Per-vertex normal information is used for lighting calculations
             * to correctly determine what contribution, if any, a particular
             * light source makes with this mesh's surface.
             * 
             * UV coordinate maps are used to map two-dimentional images onto
             * three-dimentional surfaces. They assign for each vertex a
             * coordinate pair (u,v) for each coordinate space an image can be
             * mapped into.
             * 
             * Custom vertex attributes assign arbitrary floating point data to
             * each vertex. Each vertex recieves a four pair (a,b,c,d) which can
             * be used for any purpose.
             * 
             * All of this information is optional and may be omitted; in which
             * case these functions will return NULL pointers and 0 sizes. A
             * mesh may contain multiple UV maps and custom attributes,
             * identified by an ID number.
             * 
             * There is no total number of UV maps or attribute numbers. The ID
             * is just a compact way to refer to them. Underlying meshdata
             * formats may use either strings or ID numbers internally to refer
             * to them.
             */
            virtual void get_normals(float** out_data, int* out_numfloats) = 0;
            virtual void get_uvmap(const int id, float** out_data, int* out_numfloats) = 0;
            virtual void get_attrib(const int id, float** out_data, int* out_numfloats) = 0;
            
            virtual bool has_normals() = 0;
            virtual bool has_uvmap(const int id) = 0;
            virtual bool has_attrib(const int id) = 0;
            
            /* Given a name of a UVMap or Attribute, retrieve it's ID number.
             * 
             * UVMaps and Attributes may have a name attached to them, which may
             * be more convenient than hard-coded ID numbers.
             */
            virtual int get_uvmap_id(const std::string name) = 0;
            virtual int get_attrib_id(const std::string name) = 0;
        };
        
        /* Retrieve the underlying mesh data.
         */
        IRequest* get_mesh_data(const float desired_quality, float* out_realquality);
    }
    
    //If you don't like triply nested classes, use these typedefs
    typedef IVMeshData::IRequest IVMeshDataRequest;
    typedef IVMeshData::IRequest::IListener IVMeshDataListener;
}
