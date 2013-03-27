#ifndef __SIMKIT_VRESOURCE_H__
#define __SIMKIT_VRESOURCE_H__

#include <simkit.h>
#include <string>

namespace SimKit {
    class MRefCount;
    
    /* A VResource is a resource that does not exist in memory.
     * 
     * VResources are used by loading them into memory by creating a Request.
     * The Request is responsible for actually getting the resource into memory
     * as a standard, pre-agreed-upon type (e.g. SDL_Surface for images).
     * 
     * VResources are intended to be loaded by process-wide cache objects. Thus,
     * they are reference counted, such that the cache can invalidate data from
     * image sources that are no longer needed.
     */
    template <typename resource_type>
    class SIMKIT_API ITVResource : public virtual MRefCount { /* Interface, Template class */
    protected:
        ITVResource() {};
    public:
        virtual ~ITVResource() {};
        
        /* Request objects are used to represent the actual resource that has been loaded.
         * 
         * They return an optional status and error message if loading fails.
         * 
         * The actual resources returned vary based on the VResource subclass.
         * However, the typical convention for resource ownership is that, if a
         * request is deleted before it's resources have been obtained, it
         * should also delete the resources as well.
         */
        class SIMKIT_API IRequest {
        public:
            enum RequestStatus {
                REQUEST_INVALID,
                    /* Request could not be satisfied because the parameters
                     * specified for the request are not accepted by the
                     * VResource target.
                     */
                REQUEST_NOT_STARTED,
                REQUEST_IN_PROCESS,
                    /* NOTE: Do not block on a NOT_STARTED or IN_PROCESS status.
                     * VResource types are allowed to perform processing within
                     * the main game loop.
                     */
                REQUEST_COMPLETED,
                REQUEST_FAILED
            };
            
            /* Class which recieves events relating to a resource load.
             */
            class SIMKIT_API IListener {
            protected:
                IListener();
            public:
                virtual void on_request_progress(resource_type* img, resource_type::IRequest* request, const RequestStatus update) = 0;
            };
        private:
            RequestStatus stat;

            resource_type* parent;
            IListener* listen;
        protected:
            IRequest(resource_type* parent) : stat(REQUEST_NOT_STARTED), listen(NULL), parent(parent) {};
            IRequest(resource_type* parent, IListener* listen) : stat(REQUEST_NOT_STARTED), listen(listen), parent(parent) {};
            
            virtual void set_request_status(const RequestStatus stat) {
                this->stat = stat;
                
                if (this->listen) {
                    this->listen->on_request_progress(this->parent, this, this->stat);
                }
            };
        public:
            virtual ~IRequest() = 0;
            
            /* Set a completion listener to be called when progress happens.
             * It will also be immediately called with the current request
             * status.
             */
            virtual void set_completion_function(IListener* listen) {
                this->listen = listen;
                
                if (this->listen) {
                    this->listen->on_request_progress(this->parent, this, this->stat);
                }
            };

            /* Check the request status. 
             */
            virtual RequestStatus check_request_status() {
                return this->stat;
            };

            /* Get errors, if any, accumulated from loading this image.
             * 
             * This function will return an empty string if there were no loading
             * errors. In general, you should only check for errors if the request
             * status was REQUEST_FAILED or REQUEST_INVALID.
             */
            virtual std::string get_loading_error() = 0;
        };
    }
}
