#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cctype>
#include <utility>

template<class T, size_t N>
class Array {
public:
    T operator[](size_t idx) const {
        return buffer_[idx];
    }

    T& operator[](size_t idx) {
        return buffer_[idx];
    }

    bool Empty() const {
        return N == 0;
    }

    size_t Size() const {
        return N;
    }

    size_t MaxSize() const {
        return N;
    }

    T& Front() {
        return buffer_[0];
    }

    T Front() const {
        return buffer_[0];
    }

    T& Back() {
        return buffer_[N - 1];
    }

    T Back() const {
        return buffer_[N - 1];
    }

    const T* Data() const {
        return buffer_;
    }

    T* Data() {
        return buffer_;
    }

    void Fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            buffer_[i] = value;
        }
    }

    void Swap(Array& other) {
        for (size_t i = 0; i < N; ++i) {
            std::swap(this->buffer[i], other.buffer_[i]);
        }
    }
private:
    T buffer_[N];
};

template<class T, size_t N>
static int CompareArrays(const Array<T, N>& first, const Array<T, N>& second) {
    for (size_t i = 0; i < first.Size() && i < second.Size(); ++i) {
        if (first[i] > second[i]) {
            return 1;
        } else if (first[i] < second[i]) {
            return -1;
        }
    }

    return static_cast<int>(first.size()) - static_cast<int>(second.size());
}

template<class T, size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return CompareArrays(lhs, rhs) == 0;
}

template<class T, size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return CompareArrays(lhs, rhs) < 0;
}

template<class T, size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return CompareArrays(lhs, rhs) > 0;
}

template<class T, size_t N>
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs == rhs);
}

template<class T, size_t N>
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs > rhs);
}

template<class T, size_t N>
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) {
    return !(lhs < rhs);
}

template<size_t I, class T, size_t N>
const T& Get(const Array<T, N>& array) {
    return array[I];
}

template<size_t I, class T, size_t N>
T& Get(Array<T, N>& array) {
    return array[I];
}


#endif //ARRAY_HPP
