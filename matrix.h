#include <iostream>
#include <map>
#include <array>
#include <memory>

template <unsigned int N>
using Indexes = std::array<unsigned int, N>;

template <typename data, unsigned int N>
using Container = std::map<Indexes<N>, data>;

template <typename Container, unsigned int max, unsigned int N, typename FinalSlot>
class MatrixSlotProxy {
    
    public:
        using Proxy = typename std::conditional<N == 1, FinalSlot, MatrixSlotProxy<Container, N, N - 1, FinalSlot>>::type;

        MatrixSlotProxy(Container &store, Indexes<max> &&indexes) 
            : m_store(store), m_indexes( indexes ) {}

        Proxy operator[] (unsigned int index)
        {
            std::array<unsigned int, max> indexes;
            std::copy(m_indexes.begin(), m_indexes.end(), indexes.begin());
            indexes[max - N] = index;
            return Proxy( m_store, std::move(indexes) );
        }

    private:
        Container &m_store;
        Indexes<max> m_indexes;
};

template<typename Container, typename Container::mapped_type default_value>
class FinalSlotProxy {

    public:
        using ValueType = typename Container::mapped_type;
        using KeyType = typename Container::key_type;

        FinalSlotProxy(Container &store, KeyType &&indexes) 
            : m_store(store), m_indexes( indexes ) {}

        operator ValueType() const {
            auto it = m_store.find(m_indexes);
            return it == m_store.end() ? default_value : it->second;
        }

        FinalSlotProxy &operator= (ValueType value)
        {
            if ( value == default_value ) {
                m_store.erase(m_indexes);
            } else {
                m_store[m_indexes] = value;
            }

            return *this;
        }

    private:
        Container &m_store;
        KeyType m_indexes;

};

template <typename type, unsigned int N, type value>
class Matrix {

    class iterator {
        public:
            iterator(typename Container<type, N>::iterator &&it) : iter(it) {}

            iterator operator++()
            {
                iter++;
                return *this;
            }

            bool operator!=(const iterator& other) const {
                return iter != other.iter;
            };

            typename Container<type, N>::value_type operator*() {
                return *iter;
            }

            typename Container<type, N>::iterator iter;
    };

    public:
        using Proxy = typename MatrixSlotProxy<Container<type, N>, N, N, FinalSlotProxy<Container<type, N>, value>>::Proxy;

        Proxy operator[] (unsigned int index)
        {
            Indexes<N> indexes;
            indexes[0] = index;
            return Proxy(m_slots, std::move(indexes));
        }

        int count()
        {
            return m_slots.size();
        }

        iterator begin()
        {
            return iterator(m_slots.begin());
        }

        iterator end()
        {
            return iterator(m_slots.end());
        }

    private:
        Container<type, N> m_slots;
};