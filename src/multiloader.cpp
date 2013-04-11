#include <simkit.h>
#include <algorithm>

SimKit::MultiLoader::MultiLoader() {};
SimKit::MultiLoader::~MultiLoader() {};

void SimKit::MultiLoader::add_subloader(SimKit::ILoader* ldr, int weight) {
    std::pair<int, SimKit::ILoader*> p(weight, ldr);
    
    this->m_loaders.push_back(p);
};

std::istream* SimKit::MultiLoader::open_resource(std::string name, std::ios_base::openmode mode = std::ios_base::in) {
    std::istream* out = NULL;
    int out_weight = 0;
    
    for (std::vector<std::pair<int, SimKit::ILoader*> >::iterator i = this->m_loaders.begin(); i != this->m_loaders.end(); i++) {
        if (i->first > out_weight) {
            std::istream* new_out = i->second.open_resource(name, mode);

            if (new_out) {
                delete out;
                out = new_out;
                out_weight = i->first;
            }
        }
    }

    return out;
};
