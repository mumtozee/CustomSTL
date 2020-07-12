#include "bubbleSort.h"
#include "countingSort.h"
#include "heapSort.h"
#include "insertionSort.h"
#include "mergeSort.h"
#include "quickSort.h"
#include "radixSort.h"
#include "selectionSort.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <utility>

template <class T>
void PrintArray(const T* arr, const size_t N, std::ostream& out = std::cout) {
    for (size_t i = 0; i < N; ++i) {
        out << arr[i] << ' ';
    }
    out << std::endl;
}

template <class T>
bool BiggerThan(const T& lhs, const T& rhs) {
    return lhs > rhs;
}

template <class T>
bool LessThan(const T& lhs, const T& rhs) {
    return lhs < rhs;
}

class TimeProfiler {
    const char* info_;
    clock_t start_;
    std::ostream& out_;
public:
    explicit TimeProfiler(const char* info, std::ostream& out = std::cout) : info_(info), start_(clock()), out_(out) {
    }

    ~TimeProfiler() {
        auto end = clock();
        out_ << info_ << ": " << static_cast<double>(end - start_) / CLOCKS_PER_SEC << '\n';
    }
};

int main() {
    const size_t N = 16'777'216; // 2^24
    // const size_t N = 1 << 15; // 32768 for slower sorts

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<> distr(0, N);

    int* array = new int[N];

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler1("bubble sort");
        BubbleSort(array, N, BiggerThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler2("selection sort");
        SelSort(array, N, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler3("insertion sort");
        InSort(array, N, BiggerThan);
    }


    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler4("insertion sort with binary search");
        BinaryInSort(array, N, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler5("quick sort hoare");
        QuickSort(array, N, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler6("quick sort lomuto");
        QuickSort(array, N, LessThan, false);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler6("quick sort with insertion");
        QSortOptimisation(array, 0, N - 1, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler7("merge sort");
        MergeSort(array, N, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler8("heap sort");
        HeapSort(array, N, LessThan);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler9("counting sort");
        CountingSort(array, N);
    }

    std::generate(array, array + N, [&distr, &generator]() {
        return distr(generator);
    });

    {
        TimeProfiler profiler10("radix sort");
        RadixSort(array, N);
    }

    delete[] array;
    return 0;
}

