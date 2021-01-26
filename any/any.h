#ifndef ANY_H
#define ANY_H

#include <exception>
#include <memory>
#include <utility>

class BadAnyCast : public std::exception {
    const char* what() const noexcept override {
        return "Bad any cast occurred";
    }
};

class Base {
public:
    virtual std::unique_ptr<Base> Clone() const = 0;
    virtual ~Base() = default;
};

template <class T>
class Derived : public Base {
public:
    Derived() = default;

    Derived(T value) : value_(value) {
    }

    Derived(const Derived& other) = default;

    Derived& operator=(const Derived& other) = default;

    Derived(Derived&& other) noexcept {
        Swap(other);
    }

    Derived& operator=(Derived&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~Derived() = default;

    T& Value() {
        return value_;
    }

    T& Value() const {
        return value_;
    }

    std::unique_ptr<Base> Clone() const override {
        return std::make_unique<Derived<T>>(*this);
    }

private:
    T value_;

    void Swap(Derived& other) {
        std::swap(value_, other.value_);
    }
};

class Any {
public:
    Any() : buffer_(nullptr) {
    }

    Any(const Any& other) : buffer_(other.buffer_->Clone()) {
    }

    Any& operator=(const Any& other) {
        Any(other).Swap(*this);
        return *this;
    }

    template <class T>
    Any(const T& value) : buffer_(std::make_unique<Derived<T>>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        buffer_ = std::make_unique<Derived<T>>(value);
        return *this;
    }

    Any(Any&& other) noexcept = default;

    Any& operator=(Any&& other) noexcept = default;

    ~Any() {
        Reset();
    }

    Base* Value() {
        return buffer_.get();
    }

    Base* Value() const {
        return buffer_.get();
    }

    void Swap(Any& other) {
        std::swap(buffer_, other.buffer_);
    }

    bool HasValue() const {
        return Value() != nullptr;
    }

    void Reset() noexcept {
        buffer_.reset();
    }

private:
    std::unique_ptr<Base> buffer_;
};

template <class T>
T any_cast(const Any& value) {
    auto tmp = dynamic_cast<Derived<T>*>(value.Value());
    if (tmp == nullptr) {
        throw BadAnyCast();
    }

    return tmp->Value();
}

#endif // ANY_H
