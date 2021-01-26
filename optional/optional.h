#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <exception>
#include <iostream>

class BadOptionalAccess : public std::exception {
    const char* what() const noexcept override {
        return "Bad Optional Access occured";
    }
};

template<class T>
class Optional {
    bool has_value_;
    char object_[sizeof(T)];

public:
    Optional() : has_value_(false) {
    }

    Optional(const Optional& other) : has_value_(other.has_value_) {
        if (other.has_value_) {
            new(&object_) T(reinterpret_cast<const T&>(other.object_));
        }
    }

    Optional(T value) : has_value_(true) {
        new(&object_) T(std::move(value));
    }

    Optional(Optional&& other) noexcept {
        Swap(other);
    }

    Optional& operator=(Optional other) noexcept {
        Swap(other);
        return *this;
    }

    ~Optional() {
        Reset();
    };

    const T* operator->() const noexcept {
        return reinterpret_cast<const T*>(object_);
    }

    T* operator->() noexcept {
        return reinterpret_cast<T*>(object_);
    }

    const T& operator*() const noexcept {
        return reinterpret_cast<const T&>(object_);
    }

    T& operator*() noexcept {
        return reinterpret_cast<T&>(object_);
    }

    explicit operator bool() const noexcept {
        return has_value_;
    }

    bool HasValue() const noexcept {
        return has_value_;
    }

    T& Value() {
        if (has_value_) {
            return reinterpret_cast<T&>(object_);
        } else {
            throw BadOptionalAccess();
        }
    }

    const T& Value() const {
        if (has_value_) {
            return reinterpret_cast<const T&>(object_);
        } else {
            throw BadOptionalAccess();
        }
    }

    T ValueOr(T default_value) const {
        return (has_value_) ? reinterpret_cast<const T&>(object_) : default_value;
    }

    void Swap(Optional& other) noexcept {
        std::swap(has_value_, other.has_value_);
        std::swap(reinterpret_cast<T&>(object_), reinterpret_cast<T&>(other.object_));
    }

    void Reset() noexcept {
        if (has_value_) {
            Value().~T();
            has_value_ = false;
        }
    } // destructs contained object if any

    template <class... Args>
    T& Emplace(Args&&... args) {
        Reset();
        has_value_ = true;
        new(&object_) T(std::forward<Args>(args)...);
        return Value();
    }
};

#define EMPLACE_IMPLEMENTED

#endif // OPTIONAL_H
