#ifndef RADIXSORT_H
#define RADIXSORT_H

#include <cstddef>

template <class T> // integral types only
void CountingSortWithExp(T* arr, const size_t N, const size_t exp, const size_t base = 10) {
    T* output = new T[N];
    auto* count = new size_t[base]{0};

    for (size_t i = 0; i < N; ++i) {
        ++count[(arr[i] / exp) % base];
    }

    for (size_t i = 1; i < base; ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = N - 1; i <= N - 1; --i) {
        output[count[(arr[i] / exp) % base] - 1] = arr[i];
        --count[(arr[i] / exp) % base];
    }

    std::copy(output, output + N, arr);

    delete[] output;
    delete[] count;
}

template <class T> // integral types only
void RadixSort(T* arr, const size_t N) {
    T max = *std::max_element(arr, arr + N);

    for (size_t exp = 1; max / exp > 0; exp *= 10) {
        CountingSortWithExp(arr, N, exp);
    }
}

#endif //RADIXSORT_H
