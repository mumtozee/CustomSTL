#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>

template <class T>
void SwapArgs(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
static void CopyBuffers(const T* from, T* to, size_t amount) {
    for (size_t i = 0; i < amount; ++i) {
        to[i] = from[i];
    }
}

template <class T>
static void FillWith(const T& value, size_t size, T* array) {
    for (size_t i = 0; i < size; ++i) {
        array[i] = value;
    }
}

template <class T>
class Vector {
public:
    Vector() : size_(0), capacity_(0), buffer_(nullptr) {
    }

    explicit Vector(size_t size) : size_(size), capacity_(size) {
        buffer_ = new T[capacity_]();
    }

    Vector(size_t size, const T& value) : Vector(size) {
        FillWith(value, size_, buffer_);
    }

    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        buffer_ = new T[capacity_];
        CopyBuffers(other.buffer_, buffer_, size_);
    }

    Vector& operator=(Vector other) {
        Swap(other);
        return *this;
    }

    ~Vector() {
        delete[] buffer_;
    }

    void Clear() {
        size_ = 0;
    }

    void PushBack(const T& value) {
        if (size_ >= capacity_) {
            Reallocate(IncreasedCapacity());
        }

        buffer_[size_++] = value;
    }

    void PopBack() {
        if (!Empty()) {
            --size_;
        }
    }

    void Resize(size_t new_size) {
        Resize(new_size, T());
    }

    void Resize(size_t new_size, const T& value) {
        if (new_size > size_ && new_size > capacity_) {
            Reallocate(new_size);
            FillWith(value, new_size - size_, buffer_ + size_);
        } else if (new_size > size_ && new_size <= capacity_) {
            FillWith(value, new_size - size_, buffer_ + size_);
        }
        size_ = new_size;
    }

    void Reserve(size_t new_cap = 0) {
        if (new_cap > capacity_) {
            Reallocate(new_cap);
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            Reallocate(size_);
        }
    }

    void Swap(Vector& other) {
        SwapArgs(size_, other.size_);
        SwapArgs(capacity_, other.capacity_);
        SwapArgs(buffer_, other.buffer_);
    }

    T& operator[](size_t idx) {
        return buffer_[idx];
    }

    T operator[](size_t idx) const {
        return buffer_[idx];
    }

    T& Front() {
        return buffer_[0];
    }

    T Front() const {
        return buffer_[0];
    }

    T& Back() {
        return buffer_[size_ - 1];
    }

    T Back() const {
        return buffer_[size_ - 1];
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    const T* Data() const {
        return buffer_;
    }

    T* Data() {
        return buffer_;
    }

private:
    T* buffer_;
    size_t size_;
    size_t capacity_;

    static const size_t kIncreasefactor = 2;

    void Reallocate(size_t new_capacity) {
        T* tmpBuffer = new T[new_capacity];
        CopyBuffers(buffer_, tmpBuffer, size_);
        delete[] buffer_;
        buffer_ = tmpBuffer;
        capacity_ = new_capacity;
    }

    size_t IncreasedCapacity() {
        if (capacity_ == 0) {
            return 1;
        } else {
            return kIncreasefactor * capacity_;
        }
    }
};

template <class T>
static int Compare(const Vector<T>& lhs, const Vector<T>& rhs) {
    for (size_t i = 0; i < lhs.Size() && i < rhs.Size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return 1;
        } else if (lhs[i] < rhs[i]) {
            return -1;
        }
    }

    if (lhs.Size() > rhs.Size()) {
        return 1;
    } else if (lhs.Size() < rhs.Size()) {
        return -1;
    } else {
        return 0;
    }
}

template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    return Compare(lhs, rhs) == 0;
}

template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return Compare(lhs, rhs) > 0;
}

template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs > rhs);
}

template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs > lhs;
}

template <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

#endif // VECTOR_H
