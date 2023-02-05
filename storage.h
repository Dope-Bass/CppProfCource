#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "hash.h"

using path = boost::filesystem::path;

class File {
    public:
        File(path p, std::string h, uintmax_t fileSize, size_t blockSize) 
            : m_path(p), m_fileSize(fileSize), m_blockSize(blockSize)
        {
            m_stream = std::make_shared<std::ifstream>();
            m_stream->open( p.string() );

            m_func = getHash( h );
        }
        ~File()
        {
            if ( m_stream->is_open() ) {
                m_stream->close();
            }
        }

        bool operator== (File &right)
        {
            if ( right.size() != m_fileSize ) {
                return false;
            } else {
                for ( auto i = 0; i <= m_fileSize; ++i ) {
                    if ( getBlock( i ) == right.getBlock( i ) ) {
                        continue;
                    }

                    return false;
                }

                return true;
            }
        }

        std::vector<std::string>::const_iterator
        hashBegin() const { return m_hashes.cbegin(); }

        std::vector<std::string>::const_iterator
        hashEnd() const { return m_hashes.cend(); }

        std::string getHashString()
        {
            std::string hash;
            for ( auto h : m_hashes ) {
                hash += h;
            }

            return hash;
        }

        std::string getBlock(long int b)
        {
            try {
                return m_hashes.at( b );
            }
            catch( std::out_of_range const& exc ) {
                std::string str(m_blockSize, '\0');
                if ( m_stream->read(&str[0], m_blockSize) ) {
                    m_hashes.push_back( m_func->hash( str ) );
                    if ( m_hashes.size() == std::ceil(m_fileSize / m_blockSize) ) {
                        m_stream->close();
                    }
                }

                return m_hashes.back();
            }
        }

        uintmax_t size() { return m_fileSize; }

        std::string getPath() { return m_path.string(); }

    private:

        path m_path;
        std::vector<std::string> m_hashes = {};
        uintmax_t m_fileSize;
        size_t m_blockSize;

        std::shared_ptr<std::ifstream> m_stream;
        std::unique_ptr<HashFunc> m_func;
};