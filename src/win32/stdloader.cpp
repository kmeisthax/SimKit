#include "../simkit/stdloader.h"

#include <memory>
#include <direct.h>
#include <exception>

SimKit::StdLoader::StdLoader() {
    //Get the current working directory
    //Win32 version
    this->cwd = _getcwd(NULL, 0);
};

SimKit::StdLoader::StdLoader(const char* cwd) {
    char* curdir = _getcwd(NULL, 0);
    _chdir(cwd);
    this->cwd = _getcwd(NULL, 0);
    _chdir(curdir);
    free(curdir);
};

SimKit::StdLoader::~StdLoader() {
    free((void*)this->cwd);
};

std::istream* SimKit::StdLoader::open_resource(std::string name, std::ios_base::openmode mode) {
    char* curdir = _getcwd(NULL, 0);
    _chdir(this->cwd);
    
    try {
        std::ifstream *out = new std::ifstream(name.c_str(), mode);
        _chdir(curdir);
        free(curdir);
        
        return out;
    } catch (std::exception& e) {
        _chdir(curdir);
        free(curdir);
        throw e;
    }
};
