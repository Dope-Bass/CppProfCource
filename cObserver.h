#include <memory>
#include <list>

class SomeObject;

class Follower {
    public:
        virtual ~Follower() = default;

        virtual void onAddSomeObject(std::shared_ptr<SomeObject> ptr) = 0;
        virtual void onRemove() = 0;

};

class Author {
    public:
        virtual ~Author() = default;

        void addFollower(Follower *f) {
            followers.push_back( f );
        }

        void notifyAddSomeObject(std::shared_ptr<SomeObject> ptr) {
            for ( auto f : followers ) {
                f->onAddSomeObject( ptr );
            }
        }

        void notifyRemove() {
            for ( auto f : followers ) {
                f->onRemove();
            }
        }

    private:
        std::list<Follower*> followers;
};