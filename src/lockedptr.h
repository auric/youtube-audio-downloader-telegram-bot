#pragma once

#include <mutex>

template<class Mutex, class Ptr>
class LockedPtr
{
public:
    LockedPtr(std::unique_lock<Mutex> lock, const Ptr &ptr)
        : m_lock(std::move(lock))
        , m_ptr(ptr)
    {}

    Ptr operator->() { return m_ptr; }
    auto operator*() { return *m_ptr; }
    auto get() const { return &(*m_ptr); }
    explicit operator bool() const { return bool(m_ptr); }

private:
    std::unique_lock<Mutex> m_lock;
    Ptr m_ptr;
};
