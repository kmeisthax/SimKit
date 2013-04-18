#include <simkit.h>

SimKit::ImageCache::ImageCache() {};
SimKit::ImageCache::~ImageCache() {};

SimKit::ImageCache* SimKit::ImageCache::get_image_cache() {
    return &SimKit::ImageCache::sys_imgcache;
};

SimKit::IVImage* SimKit::ImageCache::add_image_to_cache(SimKit::ILoader& load, std::string resource_name) {
    BitmapVImage *i = new BitmapVImage(load, resource_name);
    if (i->check_image_is_valid()) {
        this->add_image_to_cache(i);
    } else {
        std::string err = "Failed to load image into imagecache.\nReason: ";
        err += i->get_loading_error();
        SimKit::EmergencyError(err);
        
        delete i;
    }
};

void SimKit::ImageCache::on_delete(SimKit::MDeleteHook* killed) {
    IVImage* img = dynamic_cast<IVImage*>(killed);
    SimKit::ImageCache::CacheData &cdata = this->vimage_cache[img];
    if (cdata.surface_image) SDL_FreeSurface(cdata.surface_image);
    if (cdata.texture_image) SDL_DestroyTexture(cdata.texture_image);
    if (cdata.loaded_image) delete cdata.loaded_image;
    
    ic->vimage_cache.erase(img);
};

void SimKit::ImageCache::add_image_to_cache(SimKit::IVImage* img) {
    img.register_delete_handler(this);

    SimKit::ImageCache::CacheData chdat = {img, NULL, NULL, NULL};
    this->vimage_cache.at(img) = chdat;
};

void SimKit::ImageCache::on_request_progress(SimKit::IVImage* img, SimKit::IVImage::IRequest* request, const SimKit::IVImage::IRequest::RequestStatus update) {
    switch (update) {
        case SimKit::IVImage::IRequest::REQUEST_INVALID:
        {
            std::string err = "Image request was rejected as invalid (REQUEST_INVALID).\nReason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        }
            break;
        case SimKit::IVImage::IRequest::REQUEST_FAILED:
        {
            std::string err = "Image source failed to load (REQUEST_FAILED).\nReason: ";
            err += request->get_loading_error();
            SimKit::EmergencyError(err);
        }
            break;
        case SimKit::IVImage::IRequest::REQUEST_COMPLETED:
        {
            SimKit::ImageCache::CacheData &cdata = this->vimage_cache[vimg];
            cdata->get_request_result(&cdata.surface_image, NULL, NULL);
        }
            break;
        default:
            break;
    }
};

IVImage::RequestStatus request_image_load(IVImage* vimg, const SDL_Rect req_rect, const float req_dpi, SDL_Surface** out_surface, SDL_Rect* out_rect, float* out_dpi) {
    SimKit::ImageCache::CacheData &cdata = ic->vimage_cache[vimg];

    //Is the image load still in progress?
    if (cdata.loaded_image && cdata.loaded_image->check_request_status() == SimKit::IVImage::IRequest::REQUEST_NOT_STARTED ||
        cdata.loaded_image && cdata.loaded_image->check_request_status() == SimKit::IVImage::IRequest::REQUEST_IN_PROCESS) {
        return cdata.loaded_image->check_request_status();
    }
    
    //Is there a suitable image to return?
    if (cdata.loaded_image && cdata.surface_image) {
        SDL_Rect res_rect;
        float res_dpi;
        
        cdata.loaded_image->get_request_result(NULL, &res_rect, &res_dpi);
        
        if (req_rect.x >= res_rect->x &&
            req_rect.y >= res_rect->y &&
            req_rect.x + req_rect.w <= res_rect->x + res_rect->w &&
            req_rect.y + req_rect.h <= res_rect->y + res_rect->h &&
            req_dpi <= res_dpi) {
            //Request is a subset of loaded image result
            *out_surface = cdata.surface_image;
            *out_rect = res_rect;
            *out_dpi = res_dpi;

            return cdata.loaded_image->check_request_status();
        }
    }
    
    //Cached data is unsuitable. Tear down the existing data and make a new one.
    if (cdata.surface_image) {
        SDL_FreeSurface(cdata.surface_image);
        cdata.surface_image = NULL;
    };

    for (std::map<SDL_Renderer*, SDL_Texture*>::iterator i = cdata.texture_image.begin(); i != cdata.texture_image.end(); i++) {
        SDL_DestroyTexture(i->second);
    }
    cdata.texture_image.clear();
    
    if (cdata.loaded_image) {
        delete cdata.loaded_image;
        cdata.loaded_image = NULL;
    };
    
    cdata.loaded_image = cdata.virtual_image->request_image_load(req_rect, req_dpi, this);
    
    IVImage::RequestStatus result = cdata.loaded_image->check_request_status();
    
    if (result == SimKit::IVImage::IRequest::REQUEST_COMPLETED) {
        cdata.loaded_image->get_request_result(out_surface, out_rect, out_dpi);
    }
    
    return result;
};

IVImage::RequestStatus request_image_load(IVImage* vimg, const SDL_Rect req_rect, const float req_dpi, SDL_Renderer* render, SDL_Texture** out_texture, SDL_Rect* out_rect, float* out_dpi) {
    SimKit::ImageCache::CacheData &cdata = ic->vimage_cache[vimg];

    //Is the image load still in progress?
    if (cdata.loaded_image && cdata.loaded_image->check_request_status() == SimKit::IVImage::IRequest::REQUEST_NOT_STARTED ||
        cdata.loaded_image && cdata.loaded_image->check_request_status() == SimKit::IVImage::IRequest::REQUEST_IN_PROCESS) {
        return cdata.loaded_image->check_request_status();
    }
    
    //Is there a suitable image to return?
    if (cdata.loaded_image) {
        SDL_Rect res_rect;
        float res_dpi;
        
        cdata.loaded_image->get_request_result(NULL, &res_rect, &res_dpi);
        
        if (req_rect.x >= res_rect->x &&
            req_rect.y >= res_rect->y &&
            req_rect.x + req_rect.w <= res_rect->x + res_rect->w &&
            req_rect.y + req_rect.h <= res_rect->y + res_rect->h &&
            req_dpi <= res_dpi) {
            //Request is a subset of loaded image result
            if (cdata.texture_image.count(render) == 1) {
                *out_texture = cdata.texture_image[render];
                *out_rect = res_rect;
                *out_dpi = res_dpi;
            } else if (cdata.surface_image) {
                cdata.texture_image[render] = SDL_CreateTextureFromSurface(render, cdata.surface_image);
                *out_texture = cdata.texture_image[render];
                *out_rect = res_rect;
                *out_dpi = res_dpi;
            }
            
            return cdata.loaded_image->check_request_status();
        }
    }
    
    //Cached data is unsuitable. Tear down the existing data and make a new one.
    if (cdata.surface_image) {
        SDL_FreeSurface(cdata.surface_image);
        cdata.surface_image = NULL;
    };
    
    for (std::map<SDL_Renderer*, SDL_Texture*>::iterator i = cdata.texture_image.begin(); i != cdata.texture_image.end(); i++) {
        SDL_DestroyTexture(i->second);
    }
    cdata.texture_image.clear();
    
    if (cdata.loaded_image) {
        delete cdata.loaded_image;
        cdata.loaded_image = NULL;
    };
    
    cdata.loaded_image = cdata.virtual_image->request_image_load(req_rect, req_dpi, this);
    
    IVImage::RequestStatus result = cdata.loaded_image->check_request_status();
    
    if (result == SimKit::IVImage::IRequest::REQUEST_COMPLETED) {
        cdata.loaded_image->get_request_result(out_surface, out_rect, out_dpi);
    }
    
    return result;
};
