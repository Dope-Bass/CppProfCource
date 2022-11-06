#include <iostream>

#define BUSY true
#define FREE false

template <typename T>
class MemBlk {

public:
    MemBlk(T* p) : state(false) {
        m_ptr = p;
    }

    void setPtr(T* ptr) { m_ptr = ptr; }

    void capture() { state = BUSY; }
    void release() { state = FREE; }

    T* data() { return m_ptr; }
    bool isBusy() { return state; }

private:
    T* m_ptr;
    bool state;
};