#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <exception>

template<class T>
class WeakPtr;

class BadWeakPtr : public std::exception {
public:
    const char* what() const noexcept override {
        return "error with weak pointers occured";
    }
};

struct Counter {
    size_t strong = 0;
    size_t weak = 0;

    Counter() = default;

    Counter(size_t strong_cnt, size_t weak_cnt) :
            strong(strong_cnt), weak(weak_cnt) {
    }

    ~Counter() = default;
};

template<class T>
class SharedPtr {
public:
    SharedPtr() : buffer_(nullptr), cnt_(nullptr) {
    }

    SharedPtr(T* new_ptr) : buffer_(new_ptr), cnt_(nullptr) {
        if (new_ptr != nullptr) {
            cnt_ = new Counter(1, 0);
        }
    }

    SharedPtr(const SharedPtr& other) : buffer_(other.buffer_), cnt_(other.cnt_) {
        AddRef();
    }

    SharedPtr(const WeakPtr<T>& other) : buffer_(other.buffer_), cnt_(other.cnt_) {
        if (other.Expired()) {
            throw BadWeakPtr();
        }

        AddRef();
    }

    SharedPtr& operator=(const SharedPtr& other) {
        SharedPtr(other).Swap(*this);
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept : buffer_(other.buffer_), cnt_(other.cnt_) {
        other.buffer_ = nullptr;
        other.cnt_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~SharedPtr() {
        Reset(nullptr);
    }

    void Swap(SharedPtr& other) {
        std::swap(buffer_, other.buffer_);
        std::swap(cnt_, other.cnt_);
    }

    void Reset(T* ptr = nullptr) {
        if (cnt_ != nullptr) {
            if (cnt_->strong > 1) {
                --(cnt_->strong);
            } else {
                delete buffer_;
                if (cnt_->weak > 0) {
                    cnt_->strong = 0;
                } else {
                    delete cnt_;
                }
            }
        }

        buffer_ = ptr;
        if (ptr != nullptr) {
            cnt_ = new Counter(1, 0);
        } else {
            cnt_ = nullptr;
        }
    }

    T* Get() const noexcept {
        return buffer_;
    }

    size_t UseCount() const {
        if (cnt_ != nullptr) {
            return cnt_->strong;
        } else {
            return 0;
        }
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

    Counter* GetCounter() const {
        return cnt_;
    }

private:
    T* buffer_;
    Counter* cnt_;

    void AddRef() {
        if (cnt_) {
            ++(cnt_->strong);
        }
    }
};

template<class T>
class WeakPtr {
public:
    WeakPtr() : buffer_(nullptr), cnt_(nullptr) {
    }

    WeakPtr(const SharedPtr<T>& other) : buffer_(other.Get()), cnt_(other.GetCounter()) {
        AddRef();
    }

    WeakPtr(const WeakPtr& other) : buffer_(other.buffer_), cnt_(other.cnt_) {
        AddRef();
    }

    WeakPtr& operator=(const WeakPtr& other) {
        WeakPtr(other).Swap(*this);
        return *this;
    }

    WeakPtr(WeakPtr&& other) noexcept : buffer_(other.buffer_), cnt_(other.cnt_) {
        other.buffer_ = nullptr;
        other.cnt_ = nullptr;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~WeakPtr() {
        if (cnt_ != nullptr) {
            --(cnt_->weak);

            if (cnt_->strong == 0 && cnt_->weak == 0) {
                delete cnt_;
                buffer_ = nullptr;
            }
        }
    }

    void Swap(WeakPtr& other) {
        std::swap(cnt_, other.cnt_);
        std::swap(buffer_, other.buffer_);
    }

    void Reset() {
        WeakPtr().Swap(*this);
    }

    size_t UseCount() const {
        if (cnt_ != nullptr) {
            return cnt_->strong;
        } else {
            return 0;
        }
    }

    bool Expired() const {
        return UseCount() == 0;
    }

    SharedPtr<T> Lock() const {
        return (Expired() ? SharedPtr<T>() : SharedPtr<T>(*this));
    }

    friend SharedPtr<T>;

private:
    T* buffer_;
    Counter* cnt_;

    void AddRef() {
        if (cnt_) {
            ++(cnt_->weak);
        }
    }
};

#endif // SHARED_PTR_H
