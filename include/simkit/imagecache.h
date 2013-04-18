#ifndef __SIMKIT_IMAGECACHE_H_
#define __SIMKIT_IMAGECACHE_H_

#include <simkit.h>
#include <SDL.h>
#include <map>
#include <string>

namespace SimKit {
    class IVImage;
    class IVImage::IRequest;
    class IVImage::IRequest::IListener;
    
    /* Stores concrete surfaces derived from VImage sources.
     */
    class SIMKIT_API ImageCache : public virtual MDeleteHook::IHandler, public virtual SimKit::IVImage::IRequest::IListener {
    private:
        ImageCache();

        struct CacheData {
            IVImage* virtual_image;
            IVImage::IRequest* loaded_image;
            SDL_Surface* surface_image;
            std::map<SDL_Renderer*, SDL_Texture*> texture_image;
        };
        
        std::map<IVImage*, CacheData> vimage_cache;
        
    public:
        virtual ~ImageCache();
        
        /* Get a pointer to the system imagecache.
         */
        static ImageCache* get_image_cache();
        
        /* Add an image to the cache.
         * 
         * The image format does not matter; ImageCache will attempt to identify
         * it and initialize an appropriate IVImage for it. You may also add an
         * IVImage directly.
         * 
         * You do not own the returned IVImage instance and do not have to
         * destroy it.
         */
        IVImage* add_image_to_cache(ILoader& load, std::string resource_name);
        void add_image_to_cache(IVImage* img);
        
        /* Given an image in the cache, attempt to load it at a specified area and size.
         * 
         * The returned SDL_Surface or SDL_Texture is owned by the imagecache.
         * You should only use it to render to another surface, texture, or as
         * an image in OpenGL rendering operations. It may be destroyed upon a
         * new image load request or an explicit invalidation.
         * 
         * The image returned may be of a different size and quality than you
         * requested. You must adjust your rendering parameters as appropriate
         * to handle such a change.
         * 
         * The returned RequestStatus indicates the status of your image load.
         * If it reads REQUEST_FAILED or REQUEST_INVALID then you will not get
         * a valid surface or texture ever. If it reads REQUEST_IN_PROCESS or
         * REQUEST_NOT_STARTED, then the image is currently being loaded. In
         * this case the function may still provide you with a substitute
         * surface or texture if it exists in the cache.
         */
        IVImage::RequestStatus request_image_load(IVImage* vimg, const SDL_Rect req_rect, const float req_dpi, SDL_Surface** out_surface, SDL_Rect* out_rect, float* out_dpi);
        IVImage::RequestStatus request_image_load(IVImage* vimg, const SDL_Rect req_rect, const float req_dpi, SDL_Renderer* render, SDL_Texture** out_texture, SDL_Rect* out_rect, float* out_dpi);

        /* Invalidate all cached resources by matching resources.
         */
        void invalidate_image(IVImage* img);
        void invalidate_renderer(SDL_Renderer* render);

        /* MDeleteHook::IHandler impl */
        void on_delete(MDeleteHook* killed);
        /* IVImage::IRequest::IListener impl */
        void on_request_progress(IVImage* img, IVImage::IRequest* request, const IVImage::IRequest::RequestStatus update);
        
        static ImageCache sys_imgcache;
    };
}

#endif
