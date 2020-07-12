#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <utility>

template <class T>
void BubbleSort(T* arr, const size_t N, bool (*comparator)(const T&, const T&)) {
    for (size_t i = N - 1; i > 0; --i) {
        for (size_t j = 0; j < i; ++j) {
            if (comparator(arr[j], arr[j + 1])) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

#endif //BUBBLESORT_H
