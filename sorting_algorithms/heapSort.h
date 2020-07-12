#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <utility>

template <class T>
void SiftDown(T* arr, const size_t N, const size_t idx, bool (*is_less)(const T&, const T&)) {
    size_t i = idx;
    size_t max = idx;

    while (i < N) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < N && is_less(arr[max], arr[left])) {
            max = left;
        }

        if (right < N && is_less(arr[max], arr[right])) {
            max = right;
        }

        if (max != i) {
            std::swap(arr[max], arr[i]);
            i = max;
        } else {
            break;
        }
    }
}

template <class T>
void BuildHeap(T* arr, const size_t N, bool (*is_less)(const T&, const T&)) {
    for (size_t i = N / 2 - 1; i <= N / 2 - 1; --i) {
        SiftDown(arr, N, i, is_less);
    }
}

template <class T>
void HeapSort(T* arr, const size_t N, bool (*is_less)(const T&, const T&)) {
    BuildHeap(arr, N, is_less);

    for (size_t i = N - 1; i <= N - 1; --i) {
        std::swap(arr[i], arr[0]);
        SiftDown(arr, i, 0, is_less);
    }
}

#endif //HEAPSORT_H
