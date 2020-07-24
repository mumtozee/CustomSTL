#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

template <typename T>
struct Node {
    using NodePtr = std::unique_ptr<Node<T> >;

    T value_;
    NodePtr next_;

    Node() = default;

    explicit Node(const T& value) : value_(value), next_(nullptr) {
    }

    Node(const T& value, NodePtr to)
            : value_(value),
              next_(std::move(to)) {}

    Node(const Node& other) = delete;

    Node& operator=(const Node& other) = delete;

    Node(Node&& other) noexcept = default;

    Node& operator=(Node&& other) noexcept {
        std::swap(value_, other.value_);
        std::swap(next_, other.next_);
        return *this;
    }

    ~Node() = default;
};

template <typename T>
class ForwardList {
    using NodePtr = typename Node<T>::NodePtr;
public:
    using value_type = T;
    using size_type = std::size_t;

    class Iterator {
    public:
        Iterator() = default;

        Iterator(const NodePtr& ptr) : it_(ptr.get()) {
        }

        Iterator& operator++() {
            it_ = it_->next_.get();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            it_ = it_->next_.get();
            return tmp;
        }

        T& operator*() {
            return it_->value_;
        }

        const T& operator*() const {
            return it_->value_;
        }

        NodePtr operator->() {
            return it_;
        }

        bool operator!=(const Iterator& other) {
            return it_ != other.it_;
        }

        bool operator==(const Iterator& other) {
            return it_ == other.it_;
        }

    private:
        Node<T>* it_;
    };

    using iterator = Iterator;
    using const_iterator = Iterator;

    ForwardList() = default;

    ForwardList(const size_t size, const T& value) : size_(0) {
        for (size_t i = 0; i < size; ++i) {
            push_front(value);
        }
    }

    ForwardList(std::initializer_list<T> i_list) : size_(0) {
        for (auto it = i_list.end() - 1; it >= i_list.begin(); --it) {
            push_front(*it);
        }
    }

    ForwardList(const ForwardList& other) : size_(other.size_) {
        Node<T>* curr = other.head_.get();
        NodePtr new_head{nullptr};
        Node<T>* new_head_ptr{nullptr};

        while (curr) {
            auto tmp = std::make_unique<Node<T>>(curr->value_);

            if (!new_head) {
                new_head = std::move(tmp);
                new_head_ptr = new_head.get();
            } else {
                new_head_ptr->next_ = std::move(tmp);
                new_head_ptr = new_head_ptr->next_.get();
            }

            curr = curr->next_.get();
        }

        head_ = std::move(new_head);
    }

    ForwardList& operator=(const ForwardList& other) {
        ForwardList(other).Swap(*this);
        return *this;
    }

    ForwardList(ForwardList&& other) noexcept = default;

    ForwardList& operator=(ForwardList&& other) noexcept {
        Swap(other);
        return *this;
    };

    ~ForwardList() = default;

    iterator begin() {
        return iterator(head_);
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator cbegin() const {
        return iterator(head_);
    }

    const_iterator cend() const {
        return iterator(nullptr);
    }

    void reverse() {
        NodePtr curr = std::move(head_);
        NodePtr next;
        NodePtr prev;

        while (curr) {
            next = std::move(curr->next_);
            curr->next_ = std::move(prev);
            prev = std::move(curr);
            curr = std::move(next);
        }

        head_ = std::move(prev);
    }

    void push_front(const T& value) {
        NodePtr new_node = std::make_unique<Node<T>>(value, std::move(head_));
        head_ = std::move(new_node);
        ++size_;
    }

    void pop_front() {
        if (size_ != 0) {
            auto tmp = std::move(head_);
            head_ = std::move(tmp->next_);
            --size_;
        }
    }

    void clear() {
        while (size_ > 0) {
            pop_front();
        }
    }

    T& front() {
        return head_->value_;
    }

    const T& front() const {
        return head_->value_;
    }

    size_t size() const {
        return size_;
    }

    template <class... Args>
    void emplace_front(Args&& ... args) {
        NodePtr tmp = std::make_unique<Node<T>>(T(std::forward<Args>(args)...), std::move(head_));
        head_ = std::move(tmp);
        ++size_;
    }

private:
    size_t size_;
    NodePtr head_;

    void Swap(ForwardList& other) {
        std::swap(head_, other.head_);
        std::swap(size_, other.size_);
    }
};

#endif //FORWARDLIST_H
