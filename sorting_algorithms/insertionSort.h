#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

template <class T>
void InSort(T* arr, const size_t N, bool (*compare)(const T&, const T&)) {
    T key;
    size_t j;

    for (size_t i = 1; i < N; ++i) {
        key = arr[i];
        j = i - 1;

        while (j < i && compare(arr[j], key)) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

template <class T>
size_t BinarySearch(T* begin, T* end, const T& item, bool (*compare)(const T&, const T&)) {
    size_t left = 0;
    size_t right = end - begin;
    size_t middle;

    while (left < right) {
        middle = (left + right) / 2;

        if (!compare(begin[middle], item)) {
            right = middle;
        } else {
            left = middle + 1;
        }
    }

    return left;
}

template <class T>
void BinaryInSort(T* arr, const size_t N, bool (*compare)(const T&, const T&)) {
    T key;
    for (size_t i = 1; i < N; ++i) {
        key = arr[i];
        size_t key_idx = BinarySearch(arr, arr + i, key, compare);
        for (size_t j = i; j > key_idx; --j) {
            arr[j] = arr[j - 1];
        }

        arr[key_idx] = key;
    }
}

#endif //INSERTIONSORT_H
