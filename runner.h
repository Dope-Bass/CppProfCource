#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/operators.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include <memory>

#include <regex>

#include "storage.h"

#define HELP                "help"
#define INCLUDE_DIR         "include_dir"
#define EXCLUDE_DIR         "exclude_dir"
#define LEVEL               "level"
#define SIZE                "size"
#define MASK                "mask"
#define BLOCK_SIZE          "block_size"
#define HASH                "hash"

using path = boost::filesystem::path;
namespace f = boost::filesystem;

class Runner {
    public:
        Runner(size_t block_size, bool level, unsigned int min_size, std::string hash) 
            : m_blockSize(block_size), m_level(level), m_minSize(min_size), m_hash(hash)
        {
        }

        void setIncludeDirs(std::vector<std::string> vec)
        {
            include_dirs.clear();
            for ( auto str : vec ) {
                include_dirs.push_back( path(str) );
            }
        }

        void setExcludeDirs(std::vector<std::string> vec)
        {
            for ( auto str : vec ) {
                exclude_dirs.push_back( path(str) );
            }
        }

        void setMasks(std::vector<std::string> vec)
        {
            masks.clear();
            for ( auto str : vec ) {
                std::cout << str << std::endl;
                boost::replace_all(str, std::string("*"), std::string(".*"));
                boost::algorithm::to_lower( str );
                str.append(std::string("$"));
                std::cout << str << std::endl;
                masks.push_back( std::regex( str ) );
            }
        }

        void run()
        {
            for ( auto dir : include_dirs ) {
                if ( isDirForScan( dir ) ) {
                    run( dir );
                } else {
                    boost::format fm("%1% Argument is not a directory ");
                    throw std::invalid_argument((fm % dir).str());
                }
            }

            print();
        }
    
    private:

        void run( path p )
        {
            for ( auto &entry : boost::make_iterator_range(f::directory_iterator(p), {}) ) {
                if ( isDirForScan( entry.path() ) && m_level ) {
                    run( entry.path() );
                } else if ( f::is_regular_file(entry.path()) && regexMatch(entry.path().string())) {
                    uintmax_t size = f::file_size(entry.path());
                    if ( size > m_minSize ) {
                        push(entry.path(), size);
                    }
                }
            }
        }

        bool isDirForScan(path p)
        {
            return f::is_directory(p) && std::find(exclude_dirs.begin(), exclude_dirs.end(), p) == exclude_dirs.end();
        }

        bool regexMatch(std::string path)
        {
            std::smatch rv;
            for ( auto regex : masks ) {
                boost::algorithm::to_lower( path );
                if ( std::regex_match(path, rv, regex) ) {
                    return true;
                } else {
                    return false;
                }
            }

            return false;
        }

        void push(path p, uintmax_t size)
        {
            files.push_back(std::make_shared<File>( p, m_hash, size, m_blockSize ));

            for ( auto it = files.begin(); it != std::prev(files.end()); ++it ) {
                if ( *(*it) == *files.back() ) {
                    if ( !fileTable.contains( files.back()->getHashString() ) ) {
                        std::pair<std::string, std::string> value2( (*it)->getHashString(), (*it)->getPath() );
                        fileTable.insert( value2 );
                    }
                    std::pair<std::string, std::string> value1( files.back()->getHashString(), p.string() );
                    fileTable.insert( value1 );
                    break;
                }
            }
        }

        void print()
        {
            for ( auto bucket = 0; bucket < fileTable.bucket_count(); ++bucket ) {
                for ( auto it = fileTable.begin(bucket); it != fileTable.end(bucket); ++it ) {
                    std::cout << it->second << '\n';
                }

                if ( fileTable.begin(bucket) != fileTable.end(bucket) ) {
                    std::cout << std::endl;
                    std::cout << std::endl;
                }
            }
           
        }

        std::vector<path> include_dirs = {boost::filesystem::current_path()};
        std::vector<path> exclude_dirs = {};
        std::vector<std::regex> masks = {std::regex(".*$")};

        std::string m_hash;
        size_t m_blockSize;
        bool m_level;
        unsigned int m_minSize;

        std::list<std::shared_ptr<File>> files;
        std::unordered_multimap<std::string, std::string> fileTable;

};