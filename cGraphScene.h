#include "cDocument.h"
#include <memory>
#include <string>

class ViewObject {
    public:
        ~ViewObject() = default;

        virtual void draw() = 0;

    private:

};

class SomeViewObject : public ViewObject {
    public:
        SomeViewObject(std::shared_ptr<SomeObject> modelObj) {}
        ~SomeViewObject() = default;

        virtual void draw() override {  };

    private:
        std::shared_ptr<SomeObject> m_model;

};

class GraphScene : public Follower {
    public:
        GraphScene(Document *doc) 
        {
            doc->addFollower( this );
        }
        ~GraphScene() {}

        virtual void onAddSomeObject(std::shared_ptr<SomeObject> ptr) override
        {
            objects.push_back( std::make_shared<SomeViewObject>( ptr ) );
            draw();
        }

        virtual void onRemove() override
        {
            objects.pop_back();
            draw();
        }

        void draw()
        {
            for ( auto obj : objects ) {
                obj->draw();
            }
        }

        void erase()
        {
            objects.pop_back();
        }

    private:
        std::list<std::shared_ptr<ViewObject>> objects;
    
};