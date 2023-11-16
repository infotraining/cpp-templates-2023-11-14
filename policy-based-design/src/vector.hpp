#ifndef CLASS_TEMPLATES_VECTOR_HPP
#define CLASS_TEMPLATES_VECTOR_HPP

#include <cstddef>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <vector>


/////////////////////////////////////////////////////////////////
// RangeCheckPolicy
//

template <typename T>
concept RangeChecker = requires(T checker, size_t index, size_t size) {
    checker.check_range(index, size);
};

class ThrowingRangeChecker
{
public:
    ~ThrowingRangeChecker() = default;

    void check_range(size_t index, size_t size) const
    {
        if (index >= size)
            throw std::out_of_range("Index out of range...");
    }
};

static_assert(RangeChecker<ThrowingRangeChecker>);

/////////////////////////////////////////////////////////////////
// RangeCheckPolicy
//
class LoggingErrorRangeChecker
{
public:
    void set_log_file(std::ostream& log_file)
    {
        log_ = &log_file;
    }

    ~LoggingErrorRangeChecker() = default;

    void check_range(size_t index, size_t size) const
    {
        if ((index >= size) && (log_ != nullptr))
            *log_ << "Error: Index out of range. Index="
                  << index << "; Size=" << size << std::endl;
    }

private:
    std::ostream* log_{};
};

/////////////////////////////////////////////////////////////////
// LockingPolicy
//
template <typename T>
concept Lockable = requires(T mtx) {
    mtx.lock();
    mtx.unlock();
};

class NullMutex
{
public:
    void lock()
    {
    }

    void unlock()
    {
    }
};

/////////////////////////////////////////////////////////////////
// LockingPolicy
//
using StdLock = std::mutex;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
template <
    typename T,
    RangeChecker RangeCheckPolicy,
    Lockable LockingPolicy = NullMutex>
class Vector : public RangeCheckPolicy
{
    std::vector<T> items_;
    using mutex_type = LockingPolicy;
    mutable mutex_type mtx_;

public:
    Vector() = default;

    template <typename U>
    Vector(std::initializer_list<U> il)
        : items_{il}
    {
    }

    bool empty() const
    {
        std::lock_guard<mutex_type> lk{mtx_};
        return items_.empty();
    }

    size_t size() const
    {
        std::lock_guard<mutex_type> lk{mtx_};
        return items_.size();
    }

    const T& at(size_t index) const
    {
        std::lock_guard<mutex_type> lk{mtx_};

        RangeCheckPolicy::check_range(index, items_.size());

        return (index < items_.size()) ? items_[index] : items_.back();
    }

    void push_back(const T& item)
    {
        std::lock_guard<mutex_type> lk{mtx_};

        items_.push_back(item);
    }
};

#endif // CLASS_TEMPLATES_VECTOR_HPP
