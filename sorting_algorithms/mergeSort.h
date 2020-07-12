#ifndef MERGESORT_H
#define MERGESORT_H

template <class T>
void Merge(T* arr, size_t left, size_t mid, size_t right, bool (*is_less)(const T&, const T&)) {
    const size_t lSize = mid - left + 1;
    const size_t rSize = right - mid;
    T* L = new T[lSize];
    T* R = new T[rSize];

    for (size_t i = 0; i < lSize; ++i) {
        L[i] = arr[left + i];
    }

    for (size_t i = 0; i < rSize; ++i) {
        R[i] = arr[mid + i + 1];
    }

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while (i < lSize && j < rSize) {
        if (!is_less(R[j], L[i])) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < lSize) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < rSize) {
        arr[k] = R[j];
        ++j;
        ++k;
    }

    delete[] L;
    delete[] R;
}

template <class T>
void MergeSort(T* arr, size_t left, size_t right, bool (*is_less)(const T&, const T&)) {
    if (left < right) {
        size_t mid = (right + left) / 2;
        MergeSort(arr, left, mid, is_less);
        MergeSort(arr, mid + 1, right, is_less);
        Merge(arr, left, mid, right, is_less);
    }
}

template <class T>
void MergeSort(T* arr, const size_t N, bool (*is_less)(const T&, const T&)) {
    MergeSort(arr, 0, N - 1, is_less);
}

#endif //MERGESORT_H
