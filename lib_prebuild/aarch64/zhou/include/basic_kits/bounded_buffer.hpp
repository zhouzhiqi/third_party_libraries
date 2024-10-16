#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/bind/bind.hpp>

#include <boost/timer/timer.hpp> // for auto_cpu_timer
#include <iostream>

// Bounded Buffer Example
// The bounded buffer is normally used in a producer-consumer mode: 
// producer threads produce items and store them in the container 
// and consumer threads remove these items and process them. 

// The bounded buffer has to guarantee that
// producers do not insert items into the container when the container is full,
// consumers do not try to remove items when the container is empty,
// each produced item is consumed by exactly one consumer.

// This example shows how the circular_buffer can be utilized as an underlying container of the bounded buffer.

template<class T>
class bounded_buffer
{
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename boost::call_traits<value_type>::param_type param_type;

    explicit bounded_buffer(size_type capacity) : m_unread(0), m_container(capacity) {}

    void push_front(
        typename boost::call_traits<value_type>::param_type item
    )
    { // `param_type` represents the "best" way to pass a parameter of type `value_type` to a method.

        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
        m_container.push_front(item);
        ++m_unread;
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type *pItem)
    {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
        *pItem = m_container[--m_unread];
        lock.unlock();
        m_not_full.notify_one();
    }

private:
    bounded_buffer(const bounded_buffer &) = delete;              // Disabled copy constructor.
    bounded_buffer &operator=(const bounded_buffer &) = delete; // Disabled assign operator.

    bool is_not_empty() const { return m_unread > 0; }

    bool is_not_full() const { return m_unread < m_container.capacity(); }

    size_type m_unread;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;

}; //