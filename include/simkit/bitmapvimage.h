#ifndef __SIMKIT_BITMAPVIMAGE_H_
#define __SIMKIT_BITMAPVIMAGE_H_

#include <simkit.h>

namespace SimKit {
    /* VImage subclass that loads images through SDL2_Image library.
     */
    class SIMKIT_API BitmapVImage : virtual public IVImage {
    private:
        ILoader& load;
        std::string resource_name;
        int img_width, img_height;
        bool image_is_valid;
        std::string why_not_valid;
    public:
        BitmapVImage(ILoader& load, std::string resource_name);
        virtual ~BitmapVImage();
        
        /* IVImage impl */
        virtual bool check_image_is_valid();
        virtual std::string get_loading_error();
        virtual void get_image_size(int* out_width, int* out_height, float* out_basedpi, float* out_maxdpi) const;
        virtual IVImage::IRequest* request_image_load(const SDL_Rect src_rect, const float src_dpi);
        virtual IVImage::IRequest* request_image_load(const SDL_Rect src_rect, const float src_dpi, IVImage::IRequest::IListener* listen);
        
        class SIMKIT_API Request : virtual public IVImage::IRequest {
        private:
            SDL_Surface* result_surface;
            bool surface_requested;
            std::string why_fail;
            
            Request(BitmapVImage* parent);
            Request(BitmapVImage* parent, IVImage::IRequest::IListener* listen);
        public:
            virtual ~Request();
            
            /* IVImage::IRequest impl */
            virtual void get_request_result(SDL_Surface** out_surface, SDL_Rect* out_result_rect, float* out_result_dpi);
            virtual std::string get_loading_error();

            friend class BitmapVImage;
        };
        
        friend class BitmapVImage::Request;
    };
}

#endif
