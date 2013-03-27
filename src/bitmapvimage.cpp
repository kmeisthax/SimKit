#include <simkit.h>
#include <SDL.h>
#include <SDL_image.h>

/* Implementation of BitmapVImage which proxies into SDLImage */

SimKit::BitmapVImage::BitmapVImage(SimKit::ILoader& load, std::string resource_name) : load(load), resource_name(resource_name) {
    //this constructor automatically loads the image to extract metadata from it
    std::istream* i = this->load.open_resource(this->resource_name);
    SDL_RWops* irwops = reinterpret_cast<SDL_RWops*>(malloc(sizeof SDL_RWops));
    SimKit::construct_istream_rwops_adapter(irwops, i);
    
    SDL_Surface* img = IMG_Load_RW(irwops, 1);
    this->image_is_valid = img ? true : false;
    
    if (img) {
        this->img_width = img->w;
        this->img_height = img->h;
        free(img);
    } else {
        this->why_not_valid = IMG_GetError();
    }
};

SimKit::BitmapVImage::~BitmapVImage() {};

bool SimKit::BitmapVImage::check_image_is_valid() {
    return this->image_is_valid;
};

std::string SimKit::BitmapVImage::get_loading_error() {
    return this->why_not_valid;
};

/* IVImage impl */
void SimKit::BitmapVImage::get_image_size(int* out_width, int* out_height, float* out_basedpi, float* out_maxdpi) const {
    if (out_width) *out_width = this->img_width;
    if (out_height) *out_height = this->img_height;
    if (out_basedpi) *out_basedpi = 72.0f;
    if (out_maxdpi) *out_maxdpi = 72.0f;
};

SimKit::IVImage::IRequest* SimKit::BitmapVImage::request_image_load(const SDL_Rect src_rect, const float src_dpi) {
    return new SimKit::BitmapVImage::Request(this);
};

SimKit::IVImage::IRequest* SimKit::BitmapVImage::request_image_load(const SDL_Rect src_rect, const float src_dpi, SimKit::IVImage::IRequest::IListener* listen) {
    return new SimKit::BitmapVImage::Request(this, listen);
};

static SDL_Surface* IMG_Load_istream(std::istream* stream, int freesrc) {
    SDL_RWops* irwops = reinterpret_cast<SDL_RWops*>(malloc(sizeof SDL_RWops));
    SimKit::construct_istream_rwops_adapter(irwops, i);
    
    SDL_Surface* out = IMG_Load_RW(irwops, freesrc);
    if (freesrc == 0) free(irwops);
    return out;
};

SimKit::BitmapVImage::Request::Request(SimKit::BitmapVImage* parent) : SimKit::IVImage::IRequest(parent), result_surface(NULL), why_fail(""), surface_requested(false) {
    std::istream* i = parent->load.open_resource(parent->resource_name);

    this->result_surface = IMG_Load_istream(i, 1);
    this->set_request_status(this->result_surface ? REQUEST_COMPLETED : REQUEST_FAILED);
    
    if (!this->result_surface) {
        this->why_fail = IMG_GetError();
    }
};

SimKit::BitmapVImage::Request::Request(SimKit::BitmapVImage* parent, SimKit::IVImage::IRequest::IListener* listen) : SimKit::IVImage::IRequest(parent, listen), result_surface(NULL), why_fail(""), surface_requested(false) {
    std::istream* i = parent->load.open_resource(parent->resource_name);

    this->result_surface = IMG_Load_istream(i, 1);
    this->set_request_status(this->result_surface ? REQUEST_COMPLETED : REQUEST_FAILED);
    
    if (!this->result_surface) {
        this->why_fail = IMG_GetError();
    }
};

SimKit::BitmapVImage::Request::~Request() {
    if (this->result_surface && !this->surface_requested) {
        free(this->result_surface);
    }
};

void SimKit::BitmapVImage::Request::get_request_result(SDL_Surface** out_surface, SDL_Rect* out_result_rect, float* out_result_dpi) {
    if (out_surface) {
        *out_surface = this->result_surface;
        this->surface_requested = true;
    }
    
    if (out_result_rect) {
        out_result_rect->x = 0;
        out_result_rect->y = 0;
        out_result_rect->w = this->result_surface->w;
        out_result_rect->h = this->result_surface->h;
    }
};

std::string SimKit::BitmapVImage::Request::get_loading_error() {
    return this->why_fail;
};
