#include <string>
#include <memory>
#include "cGraphScene.h"

class Controller {
    public:
        Controller()
        {
            m_doc.reset( new Document() );
            m_scene.reset( new GraphScene(m_doc.get()) );
        }
        ~Controller();

        void addObject()
        {
            m_doc->addObject( std::shared_ptr<SomeObject>() );
        }

        void removeObject()
        {
            m_doc->rmObject();
        }

        void importDoc(const std::string &name)
        {
            m_doc.reset( new Document() );
            m_doc->deserialize( name );
        }

        void exportDoc(const std::string &name)
        {
            m_doc->serialize( name );
        }
        
    private:
        std::unique_ptr<Document> m_doc;
        std::unique_ptr<GraphScene> m_scene;
};