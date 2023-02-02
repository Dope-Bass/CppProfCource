#include <list>
#include <memory>

class Command;

using CommandPtr = std::shared_ptr<Command>;
using CmdPool = std::list<CommandPtr>;

class Follower {
    public:
        virtual ~Follower() = default;

        virtual void onExecute(CmdPool &block, unsigned long time) = 0;
};

using FPool = std::list<Follower *>;

class Author {
    public:
        virtual ~Author() = default;
        virtual void addCommand(CommandPtr command) = 0;

        void addFollower(Follower *f) 
        {
            followers.push_back( f );
        }

        void notifyExec() 
        {
            for ( auto f : followers ) {
                f->onExecute( cmds, m_blockTime );
            }
        }

    protected:
        FPool followers;
        CmdPool cmds;
        unsigned long m_blockTime;
};