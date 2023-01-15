#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <fstream>

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

class CommandSerializer {
    public:
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

        static void printBlock(std::list<std::shared_ptr<Command>> &block)
        {
            auto rv = serializeBlock(block);
            std::cout << rv;
        }

        static void logBlock(std::list<std::shared_ptr<Command>> &block, unsigned long time)
        {
            auto rv = serializeBlock(block);
            std::ofstream log;
            std::string name = std::string("bulk") + std::to_string(time) + std::string(".log");
            log.open(name);
            log << rv;
            log.close();
        }
};