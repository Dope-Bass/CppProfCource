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

    private:
        std::string m_name;
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
        CmdLogger(Author *a)
        {
            a->addFollower( this );
        }
        ~CmdLogger() {}

        virtual void onExecute(CmdPool &block, unsigned long time = 0)
        {
            auto rv = serializeBlock(block);
            std::ofstream log;
            std::string name = std::string("bulk") + std::to_string(time) + std::string(".log");
            log.open(name);
            log << rv;
            log.close();
        }
};

class CmdPrinter : public Follower {
    public:
        CmdPrinter(Author *a)
        {
            a->addFollower( this );
        }
        ~CmdPrinter() {}

        virtual void onExecute(CmdPool &block, unsigned long time)
        {
            auto rv = serializeBlock(block);
            std::cout << rv;
        }
};