#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <fstream>

#include "observer.h"

class Command {
    public:
        Command(std::string &name) 
        {
            m_name = name;
        }
        virtual std::string execute() = 0;

        std::string name() { return m_name; }
        void setTime(unsigned long time) { m_time = time; }

        unsigned long getTime() { return m_time; }

    private:
        std::string m_name;
        unsigned long m_time = 0;
};

class PrintItselfCommand : public Command {
    public:
        PrintItselfCommand(std::string &name) : Command(name) {}

        virtual std::string execute() override
        {
            return name();
        }
};

static std::string serializeBlock(std::list<std::shared_ptr<Command>> &block)
{
    std::string out = "bulk: ";
    for ( auto it = block.begin(); it != block.end(); ++it ) {
        out += (*it)->execute();
        if ( it != std::prev(block.end()) ) {
            out += ", ";
        }
    }
    out += "\n";
    return out;
}

class CmdLogger : public Follower {
    public:
        CmdLogger(Author *a) : m_author(a)
        {
            a->addFollower( this );
            m_thread = std::thread(&CmdLogger::worker, this);
        }
        ~CmdLogger() {}

    private:
        Author *m_author;
        void worker() 
        {
            CmdPool block;
            do {
                {
                    std::unique_lock<std::mutex> guard{m_author->m_mutex};
                    if ( m_author->m_blocks.empty() ) {
                        guard.unlock();
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    }

                    block = m_author->m_blocks.front();
                    m_author->m_blocks.pop();
                }

                auto rv = serializeBlock(block);
                std::ofstream log;
                std::string name = std::string("bulk") + std::to_string((*block.begin())->getTime()) + std::string(".log");
                log.open(name);
                log << rv;
                log.close();
            } while (true);
        }
};

class CmdPrinter {
    public:
        CmdPrinter(Author *a)
        {
            m_author = a;
        }
        ~CmdPrinter() {}
        
        void process()
        {
            auto th = std::thread(&CmdPrinter::worker, this);
            th.detach();
        }

    private:
        Author *m_author;
        void worker()
        {
            CmdPool cmds = m_author->getCurrentCmds();
            auto rv = serializeBlock(cmds);
            std::cout << rv;
        }

};