#ifndef COUNTINGSORT_H
#define COUNTINGSORT_H

#include <algorithm>

template <class T> // only integral types
void CountingSort(T* arr, const size_t N) {
    T max = *std::max_element(arr, arr + N);
    T min = *std::min_element(arr, arr + N);
    size_t range = max - min + 1;

    T* output = new T[N];
    auto* count = new size_t[range]();

    for (size_t i = 0; i < N; ++i) {
        ++count[arr[i] - min];
    }

    for (size_t i = 1; i < range; ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = N - 1; i <= N - 1; --i) {
        output[count[arr[i] - min] - 1] = arr[i];
        --count[arr[i] - min];
    }

    std::copy(output, output + N, arr);

    delete[] output;
    delete[] count;
}

#endif //COUNTINGSORT_H
