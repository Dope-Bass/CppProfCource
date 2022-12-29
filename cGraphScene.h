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

};

class GraphScene : public Follower {
    public:
        GraphScene(Document *doc) 
        {
            doc->addFollower( this );
        }
        ~GraphScene() {}

        virtual void onAdd(std::shared_ptr<SomeObject> ptr) override
        {
            objects.push_back( std::shared_ptr<ViewObject>() );
        }

        virtual void onRemove() override
        {
            objects.pop_back();
        }

        void draw( std::shared_ptr<ViewObject> obj )
        {
            objects.push_back( obj );
            obj->draw();
        }

        void erase()
        {
            objects.pop_back();
        }

    private:
        std::list<std::shared_ptr<ViewObject>> objects;
    
};