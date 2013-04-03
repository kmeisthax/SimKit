#ifndef __SIMKIT_RENDERER_H_
#define __SIMKIT_RENDERER_H_

#include <simkit.h>

namespace SimKit {
    class ISceneNode;

    /* Represents an entire scene to render.
     * 
     * It may include things like skyboxes or what have you.
     */
    class SIMKIT_API IScene {
        ISceneNode* m_root;
    protected:
        Scene();
        Scene(ISceneNode* root);
        
        void set_root_node(ISceneNode* root);
    public:
        virtual ~Scene() = 0;
        
        virtual ISceneNode* get_root_node();

        /* Iterator that depth-first searches through nodes in the scene.
         */
        class iterator {
        private:
            //Past the end is indicated with NULL.
            ISceneNode* cur_elem;
            //Stack of iterators up to but not including cur_elem.
            std::vector<std::vector<ISceneNode*>::iterator> i_stack;
            
            bool begin, end;
            
            static void forward();
            static void backward();
        public:
            iterator();
            iterator(iterator& copy);
            iterator(ISceneNode* base);
            iterator(ISceneNode* base, bool goto_end);
            
            iterator& operator=(iterator& f);
            
            bool operator==(iterator& f);
            bool operator!=(iterator& f);
            
            ISceneNode* operator*();
            ISceneNode* operator->();
            
            iterator& operator++();
            iterator& operator--();
            iterator& operator++(int);
            iterator& operator--(int);
            
            iterator& operator+(int fwd);
            iterator& operator-(int bck);
			/* TODO: Add these operators (if needed)
            bool operator<(iterator& f);
            bool operator>(iterator& f);
            bool operator<=(iterator& f);
            bool operator>=(iterator& f);
            */
            iterator& operator[](int fwd);
        };
        
        virtual iterator begin();
        virtual iterator end();
    };
};

#endif
