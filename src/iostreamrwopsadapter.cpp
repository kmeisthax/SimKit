#include <simkit.h>
#include <cstdio>

extern "C" {
    Sint64 static SDLCALL istream_size(struct SDL_RWops * context) {
        std::istream* i = (std::istream*)context->hidden.unknown.data1;
        
        i->clear();
        
        std::streampos oldpos = i->tellg();
        if ((int)oldpos == -1) return oldpos;
        std::streampos endpos = -1;

        try {
            i->seekg(0, std::ios_base::end);
            endpos = i->tellg();
            i->seekg(oldpos);
        } catch (std::ios_base::failure) {
        }

        if (i->fail()) {
            i->clear();
            return -1;
        }
        
        return endpos;
    };

    Sint64 static SDLCALL ostream_size(struct SDL_RWops * context) {
        std::ostream* i = (std::ostream*)context->hidden.unknown.data1;
        
        i->clear();
        
        std::streampos oldpos = i->tellp();
        if ((int)oldpos == -1) return oldpos;
        std::streampos endpos = -1;

        try {
            i->seekp(0, std::ios_base::end);
            endpos = i->tellp();
            i->seekp(oldpos);
        } catch (std::ios_base::failure) {
        }

        if (i->fail()) {
            i->clear();
            return -1;
        }

        return endpos;
    };
    
    Sint64 static SDLCALL iostream_size(struct SDL_RWops * context) {
        std::iostream* i = (std::iostream*)context->hidden.unknown.data1;

        i->clear();
        
        std::streampos oldpos = i->tellg();
        if ((int)oldpos == -1) return oldpos;
        std::streampos endpos = -1;
        
        try {
            i->seekg(0, std::ios_base::end);
            endpos = i->tellg();
            i->seekg(oldpos);
        } catch (std::ios_base::failure) {
        }
        
        if (i->fail()) {
            i->clear();
            return -1;
        }
        
        return endpos;
    };

    Sint64 static SDLCALL istream_seek(struct SDL_RWops * context, Sint64 offset, int whence) {
        std::istream* i = (std::istream*)context->hidden.unknown.data1;
        
        i->clear();
        
        try {
            switch (whence) {
                case RW_SEEK_SET:
                    i->seekg(offset, std::ios_base::beg);
                    break;
                case RW_SEEK_CUR:
                    i->seekg(offset, std::ios_base::cur);
                    break;
                case RW_SEEK_END:
                    i->seekg(offset, std::ios_base::end);
                    break;
            }
        } catch (std::ios_base::failure) {}

        if (i->fail()) i->clear();
        
        return i->tellg();
    };
    
    Sint64 static SDLCALL ostream_seek(struct SDL_RWops * context, Sint64 offset, int whence) {
        std::ostream* i = (std::ostream*)context->hidden.unknown.data1;

        i->clear();

        try {
            switch (whence) {
                case RW_SEEK_SET:
                    i->seekp(offset, std::ios_base::beg);
                    break;
                case RW_SEEK_CUR:
                    i->seekp(offset, std::ios_base::cur);
                    break;
                case RW_SEEK_END:
                    i->seekp(offset, std::ios_base::end);
                    break;
            }
        } catch (std::ios_base::failure) {}

        if (i->fail()) i->clear();
        
        return i->tellp();
    };

    Sint64 static SDLCALL iostream_seek(struct SDL_RWops * context, Sint64 offset, int whence) {
        std::iostream* i = (std::iostream*)context->hidden.unknown.data1;
        std::streampos out = -1;

        i->clear();
        
        try {
            switch (whence) {
                case RW_SEEK_SET:
                    i->seekg(offset, std::ios_base::beg);
                    break;
                case RW_SEEK_CUR:
                    i->seekg(offset, std::ios_base::cur);
                    break;
                case RW_SEEK_END:
                    i->seekg(offset, std::ios_base::end);
                    break;
            }
            
            switch (whence) {
                case RW_SEEK_SET:
                    i->seekp(offset, std::ios_base::beg);
                    break;
                case RW_SEEK_CUR:
                    i->seekp(offset, std::ios_base::cur);
                    break;
                case RW_SEEK_END:
                    i->seekp(offset, std::ios_base::end);
                    break;
            }
        } catch (std::ios_base::failure) {}
        
        out = (int)i->tellg() == -1 ? i->tellp() : i->tellg();
        
        if (i->fail()) i->clear();
        
        return out;
    };
    
    size_t static SDLCALL istream_read (struct SDL_RWops * context, void *ptr, size_t size, size_t maxnum) {
        std::istream* i = (std::istream*)context->hidden.unknown.data1;
        
        try {
            i->read((char*)ptr, size * maxnum); //who the hell designed this API
        } catch (std::ios_base::failure) {}

        std::streampos out = 0;
        if (!i->bad()) out = i->tellg();
        i->clear();
        
        return out;
    };

    size_t static SDLCALL ostream_read (struct SDL_RWops * context, void *ptr, size_t size, size_t maxnum) {
        SDL_SetError("Attempted to read from write-only std::ostream");
        return 0;
    };

    size_t static SDLCALL iostream_read (struct SDL_RWops * context, void *ptr, size_t size, size_t maxnum) {
        std::iostream* i = (std::iostream*)context->hidden.unknown.data1;
        
        try {
            i->read((char*)ptr, size * maxnum);
        } catch (std::ios_base::failure) {}

        std::streampos out = 0;
        if (!i->bad()) out = i->tellg();
        i->clear();
        
        return out;
    };
    
    size_t static SDLCALL istream_write (struct SDL_RWops * context, const void *ptr, size_t size, size_t maxnum) {
        SDL_SetError("Attempted to write to read-only std::istream");
        return 0;
    };

    size_t static SDLCALL ostream_write (struct SDL_RWops * context, const void *ptr, size_t size, size_t maxnum) {
        std::ostream* i = (std::ostream*)context->hidden.unknown.data1;
        
        try {
            i->write((char*)ptr, size * maxnum); //who the hell designed this API
        } catch (std::ios_base::failure) {}
        
        std::streampos out = 0;
        if (!i->bad()) out = i->tellp();
        i->clear();
        
        return out;
    };

    size_t static SDLCALL iostream_write (struct SDL_RWops * context, const void *ptr, size_t size, size_t maxnum) {
        std::iostream* i = (std::iostream*)context->hidden.unknown.data1;
        
        try {
            i->write((char*)ptr, size * maxnum); //who the hell designed this API
        } catch (std::ios_base::failure) {}
        
        std::streampos out = 0;
        if (!i->bad()) out = i->tellp();
        i->clear();
        
        return out;
    };

    int static SDLCALL istream_close(struct SDL_RWops * context) {
        std::istream* i = (std::istream*)context->hidden.unknown.data1;
        delete i;
        return 0;
    };
    
    int static SDLCALL ostream_close(struct SDL_RWops * context) {
        std::ostream* i = (std::ostream*)context->hidden.unknown.data1;
        int out = 0;

        try {
            i->flush();
        } catch (std::ios_base::failure) {}

        if (i->bad()) out = -1;
        
        delete i;
        return out;
    };

    int static SDLCALL iostream_close(struct SDL_RWops * context) {
        std::iostream* i = (std::iostream*)context->hidden.unknown.data1;
        int out = 0;

        try {
            i->flush();
        } catch (std::ios_base::failure) {}

        if (i->bad()) out = -1;
        
        delete i;
        return out;
    };
};

void SimKit::construct_istream_rwops_adapter(SDL_RWops *context, std::istream *adapted) {
    context->size = &istream_size;
    context->seek = &istream_seek;
    context->read = &istream_read;
    context->write = &istream_write;
    context->close = &istream_close;
    context->hidden.unknown.data1 = reinterpret_cast<void*>(adapted);
};

void SimKit::construct_ostream_rwops_adapter(SDL_RWops *context, std::ostream *adapted) {
    context->size = &ostream_size;
    context->seek = &ostream_seek;
    context->read = &ostream_read;
    context->write = &ostream_write;
    context->close = &ostream_close;
    context->hidden.unknown.data1 = reinterpret_cast<void*>(adapted);
};

void SimKit::construct_iostream_rwops_adapter(SDL_RWops *context, std::iostream *adapted) {
    context->size = &iostream_size;
    context->seek = &iostream_seek;
    context->read = &iostream_read;
    context->write = &iostream_write;
    context->close = &iostream_close;
    context->hidden.unknown.data1 = reinterpret_cast<void*>(adapted);
};
