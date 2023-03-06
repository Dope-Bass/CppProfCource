#include "async.h"
#include "reader.cpp"

namespace async {

handle_t connect(std::size_t bulk) {
    return static_cast<handle_t>( new Reader(bulk) );
}

void receive(handle_t handle, const char *data, std::size_t size) {
    Reader *r = reinterpret_cast<Reader *>( handle );

    auto str = std::string(data, size);
    size_t pos = 0;
    std::string delimiter = "\n";
    while ((pos = str.find(delimiter)) != std::string::npos) {
        auto command = str.substr(0, pos);
        std::cout << command << std::endl;
        *r << CommandPtr( new PrintItselfCommand( command ) );
        str.erase(0, pos + delimiter.length());
    }
}

void disconnect(handle_t handle) {
    Reader *r = reinterpret_cast<Reader *>( handle );

    if ( r->dynamic == true ) {
        r->executePool();
    }

    delete r;
}

}
