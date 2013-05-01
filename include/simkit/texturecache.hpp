#ifndef __SIMKIT_TEXTURECACHE_HPP__
#define __SIMKIT_TEXTURECACHE_HPP__

#include <simkit.h>

namespace SimKit {
    class IVImage;
    class IVImage::IRequest;
    class IVImage::IRequest::IListener;
    
    class ImageCache;
    
    /* Storage cache for non-SDL texture representations.
     * 
     * It relies on ImageCache to store SDL-based image representations (e.g.
     * surfaces and textures) 
     * 
     * SDL_Texture is nice, but the use of SDL_Texture is very much married to
     * the SDL 2D rendering machinery in ways that's not so helpful for people
     * who may want to use other rendering APIs. There -is- SDL_GL_BindTexture,
     * but the SDL docs aren't too clear as to what happens behind the scenes.
     * 
     * For example, what happens if I generate mipmap levels for an SDL_Texture
     * that's been bound to the GL context? Are they guaranteed to stay attached
     * to the SDL_Texture, or is the bind function allowed to bind different GL
     * textures each time? And so on and so forth.
     * 
     * I'm also not entirely sure if it's possible to extend that mechanism to
     * other rendering APIs like Direct3D or console-specific stuff. At least
     * for D3D I know there's no equivalent of BindTexture.
     * 
     * Like MeshCache this template takes a context type, a data type, and a
     * traits type. The data type represents uploaded texture data; the context
     * type represents the area in which the data is valid. Traits contains a
     * number of static functions as follows:
     * 
     * gpu_data_type gpu_traits::upload_data(gpu_context_type, SDL_Surface*, bool*)
     *   Convert an SDL surface into the non-SDL image representation implied by
     *   TTextureCache's template parameters. The bool out-pointer is set to
     *   TRUE if the upload succeeded and FALSE if it did not.
     * 
     * void gpu_traits::destroy_data(gpu_context_type, gpu_data_type)
     *   Make a previous non-SDL image representation go away.
     *   Destruction is assumed to always succeed.
     */
    template <typename gpu_context_type, typename gpu_data_type, typename gpu_traits>
    class SIMKIT_API TTextureCache : public virtual MDeleteHook::IHandler {
        struct CacheData {
            IVImage* vimg;
            
            SDL_Rect request_rect;
            float request_dpi;
            bool image_requested;
            
            std::map<gpu_context_type, gpu_data_type> hw;
        };
        
        std::map<IVImage*, CacheData> cache;
        
    protected:
        TTextureCache();
    public:
        virtual ~TTextureCache() {
            while (this->cache.size() > 0) {
                this->invalidate_image(this->cache.begin()->first);
            }
        };
        
        /* MDeleteHook::IHandler impl */
        virtual void on_delete(MDeleteHook* killed) {
            IVImage* deadman = dynamic_cast<IVImage*>(killed);
            
            if (deadman && this->cache.count(deadman) > 0) {
                this->invalidate_image(deadman);
            }
        };
        
        void add_image_to_cache(IVImage* vimg) {
            if (this->cache.count(vimg) == 0) {
                this->vimg.register_delete_handler(this);
                
                std::pair<IVImage*, CacheData> cd;
                
                cd.first = vimg;
                cd.second.vimg = vimg;
                cd.second.image_requested = false;
                
                this->cache.insert(cd);
            }
        };
        
        /* Request an image be loaded in a particular non-SDL texture format.
         * 
         * ImageCache is used to back the original SDL_Surface image load.
         */
        IVImage::RequestStatus request_texture_load(IVImage* vimg, const SDL_Rect req_rect, const float req_dpi, gpu_context_type ctxt, gpu_data_type* out_texture, SDL_Rect* out_rect, float* out_dpi) {
            this->add_image_to_cache(vimg);
            
            if (this->cache[vimg].hw.count(ctxt) > 0) {
                if (out_texture) *out_texture = this->cache[vimg].hw[ctxt];
                if (out_rect) *out_rect = this->cache[vimg].request_rect;
                if (out_dpi) *out_dpi = this->cache[vimg].request_dpi;
                
                return IVImage::REQUEST_COMPLETE;
            }
            
            SDL_Surface* ns = NULL;
            SDL_Rect rect;
            float dpi;
            
            IVImage::RequestStatus rstat = ImageCache::sys_imgcache.request_image_load(vimg, req_rect, req_dpi, &ns, &rect, &dpi);
            
            if (rstat != IVImage::REQUEST_COMPLETE) {
                return rstat;
            }
            
            bool is_done;
            gpu_data_type dat = gpu_traits(ctxt, ns, &is_done);
            
            if (is_done) {
                this->cache[vimg].hw[ctxt] = dat;
                this->cache[vimg].request_rect = rect;
                this->cache[vimg].request_dpi = dpi;
                
                if (out_texture) *out_texture = this->cache[vimg].hw[ctxt];
                if (out_rect) *out_rect = this->cache[vimg].request_rect;
                if (out_dpi) *out_dpi = this->cache[vimg].request_dpi;
                
                return IVImage::REQUEST_COMPLETE;
            } else {
                return IVImage::REQUEST_FAILED;
            }
        };
        
        void invalidate_image(IVImage* vimg) {
            if (this->cache.count(vimg) > 0) {
                for (std::map<gpu_context_type, gpu_data_type>::iterator i = this->cache[vimg].hw.begin(); i != this->cache[vimg].hw.end(); i++) {
                    gpu_traits::destroy_data(i->first, i->second);
                }
                
                this->cache.erase(vimg);
            }
        };
    }
}

#endif
