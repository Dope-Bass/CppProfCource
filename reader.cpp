#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <chrono>
#include <thread>
#include <list>

#include "pool.h"

class Reader;

using CommandPtr = std::shared_ptr<Command>;
using CmdPool = std::list<CommandPtr>;
using Printer = std::unique_ptr<CmdPrinter>;

class ReadHandler {
    
    public:
        virtual void read(Reader *, CommandPtr) = 0;
};

using HandlerPtr = std::unique_ptr<ReadHandler>;

class Reader : public Author {

    public:

        Reader(size_t count);

        ~Reader()
        {
            for ( auto f : followers ) {
                f->stop();
            }
        }

        Reader &operator << (CommandPtr command) 
        {
            m_handler->read(this, command);
            return *this;
        }

        void setHandler(HandlerPtr ptr) 
        {
            executePool();
            m_handler = std::move( ptr );
        }

        void executePool()
        {
            if ( !cmds.empty() ) {
                {
                    std::unique_lock<std::mutex> guard(m_mutex);
                    m_printer->process();
                }
                m_blocks.push( cmds );
                cmds.clear();
            }
        }

        void addCommand(CommandPtr command)
        {
            if ( cmds.empty() ) {
                auto now = std::chrono::system_clock::now();
                auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
                auto epoch = now_ms.time_since_epoch();
                auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
                command->setTime( value.count() );
            }
            cmds.push_back( command );
        }

        int poolSize() const
        {
            return cmds.size();
        }

        size_t poolMaxSize() const
        {
            return m_count;
        }

        std::string startSym {'{'};
        std::string endSym {'}'};

        bool dynamic = false;

    private:
        HandlerPtr m_handler;
        size_t m_count;

        Printer m_printer;
};

class DynamicRead : public ReadHandler {

    public:
        virtual void read(Reader *reader, CommandPtr com) override;

        private:
            unsigned int startCount = 1;
            unsigned int endCount = 0;
};

class StaticRead : public ReadHandler {

    public:
        virtual void read(Reader *reader, CommandPtr com) override {
            if ( com->name() == reader->startSym ) {
                reader->dynamic == true;
                reader->setHandler( HandlerPtr{ new DynamicRead() } );
            } else {
                reader->addCommand( com );
                if ( reader->poolSize() >= reader->poolMaxSize() ) {
                    reader->executePool();
                }
            }
        }
};

Reader::Reader(size_t count)
{
    m_handler = HandlerPtr{ new StaticRead() };
    m_count = count;

    new CmdLogger( this );
    new CmdLogger( this );

    m_printer = std::make_unique<CmdPrinter>( this );
}

void DynamicRead::read(Reader *reader, CommandPtr com)
{
    if ( com->name() == reader->endSym ) {
        if ( ++endCount == startCount ) {
            reader->dynamic = false;
            reader->setHandler( HandlerPtr{ new StaticRead() } );
        }
    } else if ( com->name() == reader->startSym ) {
        startCount++;
    } else {
        reader->addCommand( com );
    }
}
