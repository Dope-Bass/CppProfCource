#include "alloc.h"

#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class List {

    struct Node {
        Node(const T& val) : m_data(val), m_next(nullptr), m_prev(nullptr) {}
        T m_data;
        Node* m_next;
        Node* m_prev;
    };
    
    using NodeAllocator = typename Allocator::template rebind<Node>::other;

public:

    class iterator {
    public:
        iterator(Node* node) : position(node) {}

        T& operator*() 
        {
            return position->m_data;
        }

        bool operator==(const iterator& other) const 
        {
            return position == other.position;
        }

        bool operator!=(const iterator& other) const 
        {
            return position != other.position;
        }

        iterator& operator++ ()
        {
            position = position->m_next;
            return *this;
        }

    protected:
        Node *position;

        friend class List;
    };

    class reverse_iterator : public iterator {
    public:
        reverse_iterator(Node* node) : iterator(node) {}

        reverse_iterator& operator++ ()
        {
            this->position = this->position.m_prev;
            return *this;
        }
    };

    List() : m_head(nullptr), m_tail(nullptr), m_alloc() {}
    ~List() 
    {
        clearAll();
    }

    bool isEmpty() { return m_size == 0; }

    void clearAll()
    {
        auto ptr = m_head;
        while ( ptr ) {
            auto next = ptr->m_next;
            ptr->m_next = nullptr;

            m_alloc.destroy( ptr );
            m_alloc.deallocate( ptr, 1 );

            ptr = next;
        }
        
    }

    void remove(iterator position)
    {

    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(nullptr);
    }

    reverse_iterator rend()
    {
        return reverse_iterator(m_head);
    }

    iterator begin()
    {
        return iterator(m_head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    T last() 
    {
        return m_tail->m_data;
    }

    T first()
    {
        return *begin();
    }

    void push_back(const T& element)
    {
        Node* ptr = m_alloc.allocate(1);
        m_alloc.construct(ptr, element);
        
        if ( isEmpty() ) {
            m_head = ptr;
        } else {
            auto next_to_last_node = m_tail;
            m_tail->m_next = ptr;
            ptr->m_prev = next_to_last_node;
            ptr->m_next = nullptr;
        }

        m_tail = ptr;
        m_size++;
    }

    void pop_back()
    {
        auto it = rend();
        auto last = it.position;
        (++it).position->m_next = nullptr;
        m_tail = it.position;
        m_tail->m_next = nullptr;

        m_alloc.destroy( last );
        m_alloc.deallocate( last );

        m_size--;
    }

    void pop_front()
    {
        auto it = begin();
        auto first = it.position;
        (++it).position->m_prev = nullptr;
        m_head = it.position;
        m_head->m_prev = nullptr;

        m_alloc.destroy( first );
        m_alloc.deallocate( first );

        m_size--;
    }

private:

    Node* last_node() 
    {
        auto ptr = m_head; 
        while ( ptr->m_next ) {
            ptr = ptr->m_next;
        }

        return ptr;
    }

    NodeAllocator m_alloc;
    Node *m_head;
    Node *m_tail;

    std::size_t m_size = 0;
    
};