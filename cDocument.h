#include <string>
#include <memory>
#include <list>
#include "cObserver.h"

class ModelObject {
    public:
        ~ModelObject() = default;

        virtual void serialize() = 0;

    private:

};

class SomeObject : public ModelObject {
    public:
        ~SomeObject() = default;

        virtual void serialize() override {  };

    private:

};

class Document : public Author {
    public:
        ~Document() {}

        void serialize(const std::string &name)
        {
            for ( auto obj : objects ) {
                obj->serialize();
            }
        }

        static std::shared_ptr<Document> deserialize(const std::string &name)
        {
            // read from file
        }

        void addSomeObject(std::shared_ptr<SomeObject> obj) 
        {
            objects.push_back( obj );
            notifyAddSomeObject( obj );
        }

        void rmObject()
        {
            objects.pop_back();
            notifyRemove();
        }
    
    private:
        std::list<std::shared_ptr<ModelObject>> objects;
    
};