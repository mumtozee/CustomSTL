#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <functional>
#include <vector>
#include <utility>

template <
        class T,
        class Container = std::vector<T>,
        class Compare = std::less<typename Container::value_type>
> class PriorityQueue {
public:
    using container_type = Container;
    using value_compare = Compare;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    /* constructors */
    PriorityQueue() = default;

    explicit PriorityQueue(const Container& cnt) : cnt_(cnt) {
        BuildHeap();
    }

    PriorityQueue(const PriorityQueue& other) = default;

    PriorityQueue& operator=(const PriorityQueue& other) = default;

    PriorityQueue(PriorityQueue&& other) noexcept = default;

    PriorityQueue& operator=(PriorityQueue&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~PriorityQueue() = default;

    /* class methods */
    reference Top() {
        return cnt_.front();
    }

    const_reference Top() const {
        return cnt_.front();
    }

    size_type Size() const {
        return cnt_.size();
    }

    bool Empty() const {
        return Size() == 0;
    }

    void Push(const T& item) {
        cnt_.push_back(item);
        SiftUp(Size() - 1);
    }

    template <class... Args>
    void Emplace(Args&&... args) {
        cnt_.emplace_back(std::forward<Args>(args)...);
        SiftUp(Size() - 1);
    }

    void Pop() {
        std::swap(cnt_[0], cnt_[Size() - 1]);
        cnt_.pop_back();
        SiftDown(0);
    }

    void Swap(PriorityQueue& other) {
        std::swap(cnt_, other.cnt_);
    }

protected:
    Container cnt_;

private:
    void SiftDown(const size_t idx) {
        size_t i = idx;
        size_t largest = i;

        while (i < Size()) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;

            if (left < Size() && Compare(cnt_[largest], cnt_[left])) {
                largest = left;
            }

            if (right < Size() && Compare(cnt_[largest], cnt_[right])) {
                largest = right;
            }

            if (largest != i) {
                std::swap(cnt_[largest], cnt_[i]);
                i = largest;
            } else {
                break;
            }
        }
    }

    void SiftUp(const size_t idx) {
        size_t i = idx;

        while (i > 0 && Compare(cnt_[(i - 1) / 2], cnt_[i])) {
            std::swap(cnt_[(i - 1) / 2], cnt_[i]);
            i = (i - 1) / 2;
        }
    }

    void BuildHeap() {
        const size_t N = Size();
        for (size_t i = N / 2 - 1; i <= N / 2 - 1; --i) {
            SiftDown(i);
        }
    }
};

#endif //PRIORITYQUEUE_H
