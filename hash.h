#include <string>
#include <memory>
#include <functional>
#include <boost/crc.hpp>

#define fnv                 "fnv"
#define crc32               "crc32"

class HashFunc {
    public:
        virtual std::string hash( const std::string &str ) = 0;
};

class Fnv : public HashFunc {
    public:
        virtual std::string hash( const std::string &str ) override {
            return std::to_string( func(str) );
        }

    private:
        std::hash<std::string> func = {};
};

class Crc32 : public HashFunc {
    public:
        virtual std::string hash( const std::string &str ) override {
            boost::crc_32_type result;
            result.process_bytes(str.data(), str.length());
            return std::to_string( result.checksum() );
        }
};

static std::unique_ptr<HashFunc> getHash(std::string hash)
{
    if ( hash == fnv )          { return std::make_unique<Fnv>(); }
    if ( hash == crc32 )        { return std::make_unique<Crc32>(); }

    return nullptr;
}