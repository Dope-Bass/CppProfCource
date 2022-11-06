#include <iostream>
#include <list>
#include <vector>
#include <array>

#include "mem_blk.h"

template<typename T, std::size_t N>
class MemPool {

public:

    MemPool() {}
    ~MemPool()
    {
        if ( m_start ) {
            std::cout << "deallocate: [n  = " << N << "] " << std::endl;
            
            std::free(m_start);
        }
    }

    T* reserve(std::size_t n)
    {
        if ( m_start != nullptr ) {
            if ( !isEnough( n ) ) {
                return nullptr;
            } 

            return findPlace( n );
        } else {
            std::cout << "allocate: [n = " << N << "]" << std::endl;

            auto ptr = std::malloc(N * sizeof(T));
            if (!ptr)
                throw std::bad_alloc();

            m_start = reinterpret_cast<T*>(ptr);

            for ( auto p = m_start; p < m_start + (N * sizeof(T)); p += sizeof(T) ) {
                m_blocks.push_back(MemBlk<T>(p));
            }

            m_blocks.front().capture();
            return m_start;
        }
    }

    void free(T* ptr, std::size_t n) 
    {
        freeMany(ptr, n);
    }

    bool isEnough(std::size_t for_alloc)
    {
        return freeCount() >= for_alloc;
    }

    bool isEmpty()
    {
        return freeCount() == N;
    }

    MemBlk<T> *getOwner(T* ptr)
    {
        for ( auto &blk : m_blocks ) {
            if ( blk.data() == ptr ) {
                return &blk;
            }
        }

        return nullptr;
    }

    T* start() { return m_start; }

private:

    std::size_t freeCount() 
    {
        std::size_t count = 0;
        for ( auto blk : m_blocks ) {
            if ( !blk.isBusy() ) {
                count++;
            }
        }

        return count;
    }

    T* findPlace(std::size_t n)
    {
        std::size_t count = 0;
        T* placePtr = nullptr;
        int current = 0;
        while ( current < m_blocks.size() ) {
            if ( !m_blocks[current].isBusy() ) {
                placePtr = m_blocks[current].data();
            }
            while ( !m_blocks[current].isBusy() ) {
                count++;
                current++;
                if ( count == n ) {
                    captureMany( placePtr, count );
                    return placePtr;
                }
            }
            count = 0;
            current++;
            placePtr = nullptr;
        }

        return nullptr;
    }

    void captureMany(T* start, std::size_t count)
    {
        for ( auto ptr = start; ptr != (start + count*sizeof(T)); ptr += sizeof(T) ) {
            if ( auto found = getOwner( ptr ); found ) {
                found->capture();
            }
        }
    }

    void freeMany(T* start, std::size_t count)
    {
        for ( auto ptr = start; ptr != (start + count*sizeof(T)); ptr += sizeof(T) ) {
            if ( auto found = getOwner( ptr ); found ) {
                found->release();
            }
        }
    }

    T* m_start = nullptr;

    std::vector<MemBlk<T>> m_blocks;
};