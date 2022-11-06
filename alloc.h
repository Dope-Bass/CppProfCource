#include <iostream>
#include <map>
#include <list>
#include <algorithm>

#include "mem_pool.h"

template <typename T, size_t N>
class Allocator {

public:
    using value_type = T;

	using pointer = T *;
	using const_pointer = const T *;
	using reference = T &;
	using const_reference = const T &;

	template <typename U>
	struct rebind {
		using other = Allocator<U, N>;
	};

    Allocator() = default;
	~Allocator() = default;

	template <typename U>
	Allocator(const Allocator<U, N> &) {}

    T* allocate(std::size_t n) 
    {
		T* reserved = nullptr;

		for ( auto it = m_pools.begin(); it != m_pools.end() && !reserved; ++it ) {
			reserved = it->reserve( n );
		}

		if ( reserved ) {
			return reserved;
		}

		m_pools.emplace_back();

		return m_pools.back().reserve( n );
    }

	void deallocate(T *ptr, std::size_t n) {
		auto pool_it = std::find_if(m_pools.begin(), m_pools.end(), [ptr](auto &pool){ return pool.getOwner( ptr ); });

		if ( pool_it != m_pools.end() ) {
			pool_it->free( ptr, n );

			if ( pool_it->isEmpty() ) {
				m_pools.erase( pool_it );
			}
		}
	}

	template <typename U, typename... Args>
	void construct(U *ptr, Args &&...args) {
		std::cout << "construct" << std::endl;

		new (ptr) U(std::forward<Args>(args)...);
	};

	template <typename U>
	void destroy(U *ptr) {
		std::cout << "destroy" << std::endl;

		ptr->~U();
	}

private:

	std::list<MemPool<T, N>> m_pools;
	
};