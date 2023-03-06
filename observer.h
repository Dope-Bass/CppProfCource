#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

class Command;

using CommandPtr = std::shared_ptr<Command>;
using CmdPool = std::list<CommandPtr>;
using BlockPool = std::queue<CmdPool>;

class Follower {
    public:
        virtual ~Follower() = default;

        std::thread m_thread;

        void stop()
        {
            m_thread.join();
        }

    protected:
        virtual void worker() = 0;
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

        void stop()
        {
            for ( auto f : followers ) {
                f->m_thread.join();
            }
        }

        CmdPool getCurrentCmds()
        {
            return cmds;
        }

        std::mutex m_mutex;
        BlockPool m_blocks = {};

    protected:
        FPool followers;
        CmdPool cmds;
        unsigned long m_blockTime;
        
};