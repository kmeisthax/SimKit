#ifndef __SIMKIT_VIMAGE_H_
#define __SIMKIT_VIMAGE_H_

#include <simkit.h>
#include <SDL.h>
#include <map>

namespace SimKit {
    class MRefCount;
    
    /* Virtual image class used to represent a source of image/texture data.
     * 
     * A virtual image can be turned into a concrete SDL_Surface/GL texture/etc
     * at a requested size as needed. Data is filled into a target surface or
     * structure and made available to consuming code.
     * 
     * The virtual image API is designed to support flexible image sources, i.e.
     * it can load from plain images, vector graphics, or multi-level image
     * stores to support Unreal-style dynamic texture loading.
     */
    class SIMKIT_API IVImage : public virtual ITVResource<IVImage> {
    protected:
        IVImage();
    public:
        virtual ~IVImage() = 0;
        
        virtual bool check_image_is_valid() = 0;
        /* If there was an error loading
         */
        virtual std::string get_loading_error() = 0;

        /* Get image size metadata.
         * 
         * Width and height set the limits of the request rectangle.
         * BaseDPI is the DPI that the width/height figures are specified in,
         * i.e. to get an image of exactly widthxheight, request at basedpi.
         * MaxDPI is the highest resolution that can be loaded; if this is a
         * vector image source this should be infinite.
         * 
         * DPI parameters do not have to represent literal DPI sizes, say, in
         * the case of textures for 3D rendering.
         */
        virtual void get_image_size(int* out_width, int* out_height, float* out_basedpi, float* out_maxdpi) const = 0;
        
        /* Request class that tracks the status of an image load.
         */
        class SIMKIT_API IRequest : public virtual ITVResource<IVImage>::IRequest {
        public:
            typedef ITVResource<IVImage>::IRequest::IListener IListener;
        protected:
            IRequest(IVImage* parent);
            IRequest(IVImage* parent, IListener* listen);
        public:
            virtual void get_request_result(SDL_Surface** out_surface, SDL_Rect* out_result_rect, float* out_result_dpi) = 0;
        };
        
        /* Request that an image (or part of it) is loaded into an SDL_Surface.
         * 
         * A request specifies how much of the image is needed and how much
         * resolution is needed. Resolution must not exceed maximum resolution
         * specified in get_image_size.
         */
        virtual IRequest* request_image_load(const SDL_Rect src_rect, const float src_dpi) = 0;
        virtual IRequest* request_image_load(const SDL_Rect src_rect, const float src_dpi, IRequest::IListener* listen) = 0;
    };
    
    //If you don't like triply nested classes, use these typedefs
    typedef IVImage::IRequest IVImageRequest;
    typedef IVImage::IRequest::IListener IVImageListener;
}

#endif
