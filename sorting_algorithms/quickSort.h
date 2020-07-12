#ifndef QUICKSORT_H
#define QUICKSORT_H

template <class T>
long long Lomuto(T* arr, const long long low, const long long high, bool (*is_less)(const T&, const T&)) {
    long long mid = (high + low) / 2;

    if (is_less(arr[mid], arr[low])) {
        std::swap(arr[low], arr[mid]);
    }

    if (is_less(arr[high], arr[low])) {
        std::swap(arr[low], arr[high]);
    }

    if (is_less(arr[mid], arr[high])) {
        std::swap(arr[mid], arr[high]);
    }

    T pivot = arr[high];
    long long i = low;

    for (long long j = low; j <= high; ++j) {
        if (is_less(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[high]);

    return i;
}

template <class T>
long long Hoare(T* arr, const long long low, const long long high, bool (*is_less)(const T&, const T&)) {
    long long i = low - 1;
    long long j = high + 1;
    T pivot = arr[low + (high - low) / 2];

    while (true) {
        do {
            ++i;
        } while (is_less(arr[i], pivot));

        do {
            --j;
        } while (is_less(pivot, arr[j]));

        if (i >= j) {
            return j;
        }

        std::swap(arr[i], arr[j]);
    }
}

template <class T>
void QuickSortHoare(T* arr, const long long low, const long long high, bool (*is_less)(const T&, const T&)) {
    if (low < high) {
        long long p = Hoare(arr, low, high, is_less);
        QuickSortHoare(arr, low, p, is_less);
        QuickSortHoare(arr, p + 1, high, is_less);
    }
}

template <class T>
void QuickSortLomuto(T* arr, const long long low, const long long high, bool (*is_less)(const T&, const T&)) {
    if (low < high) {
        long long p = Lomuto(arr, low, high, is_less);
        QuickSortLomuto(arr, low, p - 1, is_less);
        QuickSortLomuto(arr, p + 1, high, is_less);
    }
}

template <class T>
void InsertionSort(T* arr, long long low, long long high, bool (*is_less)(const T&, const T&)) {
    for (long long i = low + 1; i <= high; ++i) {
        T key = arr[i];
        long long j = i - 1;

        while (j >= low && is_less(key, arr[j])) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

template <class T>
void QSortOptimisation(T* arr, long long low, long long high, bool (*is_less)(const T&, const T&)) {
    while (low < high) {
        if (high - low < 32) {
            InsertionSort(arr, low, high, is_less);
            high = low;
        } else {
            auto p = Hoare(arr, low, high, is_less);

            if (p > low + (high - low) / 2) {
                QSortOptimisation(arr, p + 1, high, is_less);
                high = p;
            } else {
                QSortOptimisation(arr, low, p, is_less);
                low = p + 1;
            }
        }
    }
}

template <class T>
void QuickSort(T* arr, const long long N, bool (*is_less)(const T&, const T&), bool IsHoare = true) {
    if (IsHoare) {
        QuickSortHoare(arr, 0, N - 1, is_less);
    } else {
        QuickSortLomuto(arr, 0, N - 1, is_less);
    }
}

#endif //QUICKSORT_H
