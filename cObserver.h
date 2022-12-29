#include <memory>
#include <list>

class SomeObject;

class Follower {
    public:
        virtual ~Follower() = default;

        virtual void onAdd(std::shared_ptr<SomeObject> ptr) = 0;
        virtual void onRemove() = 0;

};

class Author {
    public:
        virtual ~Author() = default;

        void addFollower(Follower *f) {
            followers.push_back( f );
        }

        void notifyAdd(std::shared_ptr<SomeObject> ptr) {
            for ( auto f : followers ) {
                f->onAdd( ptr );
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