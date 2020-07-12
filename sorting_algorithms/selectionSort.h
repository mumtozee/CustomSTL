#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <utility>

template <class T>
void SelSort(T* arr, const size_t N, bool (*is_less)(const T&, const T&)) {
    for (size_t i = 0; i < N - 1; ++i) {
        size_t min_idx = i;

        for (size_t j = i + 1; j < N; ++j) {
            if (is_less(arr[j], arr[min_idx])) {
                min_idx = j;
            }
        }

        std::swap(arr[i], arr[min_idx]);
    }
}

#endif //SELECTIONSORT_H
