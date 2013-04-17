#include <simkit.h>

#include <map>

static std::map<SDL_Window*, SDL_GLContext> last_good_context;
static std::vector<SDL_GLContext> known_contexts;
static std::map<SDL_GLContext, SDL_Window*> last_used_window;
static std::vector<SDL_Window*> known_windows;

int SimKit::OpenGL::make_current(SDL_Window* wnd, SDL_GLContext* out_selected_ctxt) {
    int err_code = -100;
    
    if (last_good_context.count(wnd) != 0) {
        if ((err_code = SDL_GL_MakeCurrent(wnd, last_good_context[wnd])) == 0) {
            last_used_window[last_good_context[wnd]] = wnd;
            if (out_selected_ctxt) *out_selected_ctxt = last_good_context[wnd];
            
            LoadFunctions();
            
            return err_code;
        }
    }
    
    for (std::vector<SDL_GLContext>::iterator i = known_contexts.begin(); i != known_contexts.end(); i++) {
        if ((err_code = SDL_GL_MakeCurrent(wnd, *i)) == 0) {
            last_good_context[wnd] = *i;
            last_used_window[*i] = wnd;
            if (out_selected_ctxt) *out_selected_ctxt = *i;
            
            LoadFunctions();
            
            return err_code;
        }
    }
    
    SDL_GLContext new_ctxt = SDL_GL_CreateContext(wnd);
    
    if (new_ctxt != NULL) {
        known_contexts.push_back(new_ctxt);
        last_good_context[wnd] = new_ctxt;
        last_used_window[new_ctxt] = wnd;
        if (out_selected_ctxt) *out_selected_ctxt = new_ctxt;
        
        LoadFunctions();
        
        return 0;
    }
    
    if (out_selected_ctxt) *out_selected_ctxt = NULL;
    return err_code;
};

int SimKit::OpenGL::make_current(SDL_Window* wnd, SDL_GLContext ctxt) {
    int err_code = SDL_GL_MakeCurrent(wnd, ctxt);
    
    if (err_code == 0) {
        last_good_context[wnd] = ctxt;
        last_used_window[ctxt] = wnd;

        if (known_contexts.count(ctxt) == 0) known_contexts.push_back(ctxt);
        
        LoadFunctions();
    }
    
    return err_code;
};

int SimKit::OpenGL::make_current(SDL_GLContext ctxt) {
    int err_code = -100;
    
    if (last_used_window.count(ctxt) > 0 && (err_code = SDL_GL_MakeCurrent(last_used_window[ctxt], ctxt)) == 0) {
        last_good_context[i->second] = ctxt;
        if (known_contexts.count(ctxt) == 0) known_contexts.push_back(ctxt);
        
        LoadFunctions();
        
        return err_code;
    }
    
    for (std::vector<SDL_Window*>::iterator i = known_windows.begin(); i != known_windows.end(); i++) {
        if ((err_code = SDL_GL_MakeCurrent(*i, ctxt)) == 0) {
            last_good_context[wnd] = ctxt;
            last_used_window[ctxt] = wnd;
            if (known_contexts.count(ctxt) == 0) known_contexts.push_back(ctxt);
            
            LoadFunctions();

            return err_code;
        }
    }
    
    return err_code;
};

void SimKit::OpenGL::invalidate_window(SDL_Window* wnd) {
    for (std::map<SDL_GLContext, SDL_Window*>::iterator i = last_used_window.begin(); i != last_used_window.end(); i++) {
        if (i == last_used_window.begin() && i->second == wnd) {
            last_used_window.erase(i);
            i = last_used_window.begin();
        } else if (i->second == wnd) {
            std::map<SDL_GLContext, SDL_Window*>::iterator itmp = i;
            --itmp;
            
            last_used_window.erase(i);
            i = itmp;
        }
        
        if (i == last_used_window.end()) break;
    }
    
    if (known_windows.count(wnd) > 0) {
        for (std::vector<SDL_Window*>::iterator i = known_windows.begin(); i != known_windows.end(); i++) {
            if (*i == wnd) i = known_windows.erase(i);
            if (i == known_windows.end()) break;
        }
    }
    
    if (last_good_context.count(wnd) > 0) last_good_context.erase(wnd);
};

void SimKit::OpenGL::invalidate_context(SDL_GLContext ctxt) {
    for (std::map<SDL_Window*, SDL_GLContext>::iterator i = last_good_context.begin(); i != last_good_context.end(); i++) {
        if (i == last_good_context.begin() && i->second == ctxt) {
            last_good_context.erase(i);
            i = last_good_context.begin();
        } else if (i->second == ctxt) {
            std::map<SDL_Window*, SDL_GLContext>::iterator itmp = i;
            --itmp;
            
            last_good_context.erase(i);
            i = itmp;
        }
        
        if (i == last_good_context.end()) break;
    }
    
    if (known_contexts.count(ctxt) > 0) {
        for (std::vector<SDL_Window*>::iterator i = known_contexts.begin(); i != known_contexts.end(); i++) {
            if (*i == ctxt) i = known_contexts.erase(i);
            if (i == known_contexts.end()) break;
        }
    }
    
    if (last_used_window.count(ctxt) > 0) last_used_window.erase(ctxt);
};
