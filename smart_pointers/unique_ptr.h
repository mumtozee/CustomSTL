#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template<class T>
class UniquePtr {
public:
    UniquePtr() : buffer_(nullptr) {
    }

    explicit UniquePtr(T* new_ptr) : buffer_(new_ptr) {
    }

    UniquePtr(const UniquePtr& other) = delete;

    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) noexcept : buffer_(other.buffer_) {
        other.buffer_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (&other == this) {
            return *this;
        }

        buffer_ = other.buffer_;
        other.buffer_ = nullptr;
        return *this;
    }

    ~UniquePtr() {
        delete buffer_;
    }

    T* Release() {
        T* tmp = buffer_;
        buffer_ = nullptr;
        return tmp;
    }

    void Reset(T* ptr = nullptr) {
        delete buffer_;
        buffer_ = ptr;
    }

    T* Get() const noexcept {
        return buffer_;
    }

    void Swap(UniquePtr& other) {
        std::swap(other.buffer_, buffer_);
    }

    T& operator*() const {
        return *buffer_;
    }

    T* operator->() const {
        return buffer_;
    }

    explicit operator bool() const {
        return buffer_ != nullptr;
    }

private:
    T* buffer_;
};

#define MAKE_UNIQUE_IMPLEMENTED

template<class T, class... Args>
UniquePtr<T> MakeUnique(Args&& ... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

#endif //UNIQUE_PTR_H
