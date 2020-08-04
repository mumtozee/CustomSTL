#ifndef BST_H
#define BST_H

#include <functional>
#include <iostream>
#include <stack>
#include <utility>

template <class T>
struct Node {
    T val_;
    Node* left_;
    Node* right_;
    Node* parent_;

    Node() = default;

    explicit Node(const T& data,
                  Node* left = nullptr,
                  Node* right = nullptr,
                  Node* parent = nullptr)
            : val_(data),
              left_(left),
              right_(right),
              parent_(parent) {}

    Node(const Node& other) = default;

    Node(Node&& other) noexcept
            : val_(other.val_),
              left_(other.left_),
              right_(other.right_),
              parent_(other.parent_) {}

    Node& operator=(const Node& other) {
        Node(other).Swap(*this);
        return *this;
    }

    Node& operator=(Node&& other) {
        Swap(other);
        return *this;
    }

    virtual ~Node() = default;

private:
    virtual void Swap(Node& other) {
        std::swap(val_, other.val_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(parent_, other.parent_);
    }
};

template <class T>
bool isLeftChild(Node<T>* node) {
    if (node->parent_ != nullptr) {
        return (node == node->parent_->left_);
    }
    return false;
}

template <class T>
bool isRightChild(Node<T>* node) {
    if (node->parent_ != nullptr) {
        return (node == node->parent_->right_);
    }
    return false;
}

template <class T>
bool isLeaf(Node<T>* node) {
    return (node->left_ == nullptr && node->right_ == nullptr);
}

template <class T>
Node<T>* SubtreeMinimum(Node<T>* root) {
    auto curr = root;
    while (curr != nullptr && curr->left_ != nullptr) {
        curr = curr->left_;
    }
    return curr;
}

template <class T>
Node<T>* SubtreeMaximum(Node<T>* root) {
    auto curr = root;
    while (curr != nullptr && curr->right_ != nullptr) {
        curr = curr->right_;
    }
    return curr;
}

template <class T>
Node<T>* LeafSuccessor(Node<T>* node) {
    Node<T>* curr = node;
    if (curr->right_ != nullptr) {
        return SubtreeMinimum(curr->right_);
    } else {
        while (curr->parent_ != nullptr && isRightChild(curr)) {
            curr = curr->parent_;
        }
    }
    return curr->parent_;
}

template <class T>
Node<T>* LeafPredecessor(Node<T>* node) {
    Node<T>* curr = node;
    if (curr->left_ != nullptr) {
        return SubtreeMaximum(curr->left_);
    } else {
        while (curr->parent_ != nullptr && isLeftChild(curr)) {
            curr = curr->parent_;
        }
    }
    return curr->parent_;
}

template <
        class T,
        class Compare = std::less<T>
>
class BST {
    using nptr = Node<T>*;
public:
    template <bool isConst = false>
    class Iterator {
    public:
        using value_type = T;
        using reference = typename std::conditional<isConst, const T&, T&>::type;
        using pointer = typename std::conditional<isConst, const T*, T*>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t;

        explicit Iterator(nptr point = nullptr, nptr root = nullptr)
                : current_(point),
                  root_node_(root) {}

        Iterator(const Iterator& other) = default;

        Iterator(Iterator&& other) noexcept = default;

        Iterator& operator=(const Iterator& other) = default;

        Iterator& operator=(Iterator&& other) noexcept = default;

        ~Iterator() = default;

        reference operator*() {
            return current_->val_;
        }

        pointer operator->() {
            return &current_->val_;
        }

        Iterator& operator++() {
            current_ = LeafSuccessor(current_);
            return *this;
        }

        Iterator& operator--() {
            if (current_ == nullptr) {
                current_ = SubtreeMaximum(root_node_);
                return *this;
            }

            current_ = LeafPredecessor(current_);

            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        Iterator operator--(int) {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        bool operator!=(const Iterator& other) {
            return current_ != other.current_;
        }

    private:
        nptr current_;
        nptr root_node_;
    };

    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    /* constructor definitions */
    BST() : root_(nullptr), size_(0) {
    }

    BST(std::initializer_list<T> init) : BST() {
        for (const auto& item : init) {
            Insert(item);
        }
    }

    BST(const BST& other) : root_(nullptr), size_(other.size_) {
        if (other.root_ == nullptr) {
            return;
        }

        nptr other_root = other.root_;
        root_ = new Node<T>(other_root->val_);
        nptr new_root = root_;

        while (other_root != nullptr) {
            if (other_root->left_ != nullptr && new_root->left_ == nullptr) {
                new_root->left_ = new Node<T>(other_root->left_->val_, nullptr, nullptr, new_root);
                other_root = other_root->left_;
                new_root = new_root->left_;
            } else if (other_root->right_ != nullptr && new_root->right_ == nullptr) {
                new_root->right_ = new Node<T>(other_root->right_->val_, nullptr, nullptr, new_root);
                other_root = other_root->right_;
                new_root = new_root->right_;
            } else {
                new_root = new_root->parent_;
                other_root = other_root->parent_;
            }
        }
    }

    BST(BST&& other) noexcept = default;

    BST& operator=(const BST& other) {
        BST(other).Swap(*this);
        return *this;
    }

    BST& operator=(BST&& other) noexcept {
        Swap(other);
        return *this;
    }

    virtual ~BST() {
        Clear();
    }

    /* class methods' definitions */
    iterator begin() {
        return iterator(GetMinimum(), root_);
    }

    const_iterator cbegin() const {
        return const_iterator(GetMinimum(), root_);
    }

    iterator end() {
        nptr last = GetMaximum();
        if (last == nullptr) {
            return iterator(nullptr, root_);
        }
        return iterator(last->right_, root_);
    }

    const_iterator cend() const {
        nptr last = GetMaximum();
        if (last == nullptr) {
            return const_iterator(nullptr, root_);
        }
        return const_iterator(last->right_, root_);
    }

    std::reverse_iterator<iterator> rbegin() {
        return std::make_reverse_iterator<iterator>(end());
    }

    std::reverse_iterator<const_iterator> crbegin() const {
        return std::make_reverse_iterator<const_iterator>(cend());
    }

    std::reverse_iterator<iterator> rend() {
        return std::make_reverse_iterator<iterator>(begin());
    }

    std::reverse_iterator<const_iterator> crend() const {
        return std::make_reverse_iterator<const_iterator>(cbegin());
    }

    nptr GetMinimum() const {
        return SubtreeMinimum(root_);
    }

    nptr GetMaximum() const {
        return SubtreeMaximum(root_);
    }

    iterator Find(const_reference item) {
        auto pos = Search(item);
        if (pos == nullptr) {
            return end();
        }
        return iterator(pos, root_);
    }

    const_iterator Find(const_reference item) const {
        auto pos = Search(item);
        if (pos == nullptr) {
            return cend();
        }
        return const_iterator(pos, root_);
    }

    void Print() const {
        nptr curr = root_;
        std::stack<nptr> visited;

        while (curr != nullptr || !visited.empty()) {
            while (curr != nullptr) {
                visited.push(curr);
                curr = curr->left_;
            }

            curr = visited.top();
            visited.pop();
            std::cout << curr->val_ << ' ';
            curr = curr->right_;
        }

        std::cout << '\n';
    }

    void Clear() {
        auto curr = GetMinimum();
        while (curr != nullptr && !isLeaf(root_)) {
            if (isLeaf(curr) && isLeftChild(curr)) {
                auto succ = LeafSuccessor(curr);
                delete curr;
                succ->left_ = nullptr;
                curr = succ;
            } else if (isLeaf(curr) && isRightChild(curr)) {
                auto pre = LeafPredecessor(curr);
                delete curr;
                pre->right_ = nullptr;
                curr = pre;
            } else if (!isLeaf(curr)) {
                curr = LeafSuccessor(curr);
            }
        }
        delete root_;
        root_ = nullptr;
        size_ = 0;
    }

    void Insert(const T& item) {
        auto new_node = new Node<T>(item);
        nptr tmp_parent = nullptr;
        nptr curr = root_;

        while (curr != nullptr) {
            tmp_parent = curr;

            if (Compare{}(new_node->val_, curr->val_)) {
                curr = curr->left_;
            } else {
                curr = curr->right_;
            }
        }

        new_node->parent_ = tmp_parent;
        if (tmp_parent == nullptr) {
            root_ = new_node;
        } else if (Compare{}(new_node->val_, tmp_parent->val_)) {
            tmp_parent->left_ = new_node;
        } else {
            tmp_parent->right_ = new_node;
        }
        ++size_;
    }

    void Delete(const T& item) {
        nptr node = Search(item);

        if (node == nullptr) {
            return;
        }

        if (node->left_ == nullptr) {
            Transplant(node, node->right_);
        } else if (node->right_ == nullptr) {
            Transplant(node, node->left_);
        } else {
            nptr succ = SubtreeMinimum(node->right_);

            if (node != succ->parent_) {
                Transplant(succ, succ->right_);
                succ->right_ = node->right_;
                succ->right_->parent_ = succ;
            }

            Transplant(node, succ);
            succ->left_ = node->left_;
            succ->left_->parent_ = succ;
        }

        delete node;
        node = nullptr;
        --size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_type Size() const {
        return size_;
    }

protected:
    nptr root_;
    size_type size_;

    nptr Search(const_reference key) const {
        nptr curr = root_;

        while (curr != nullptr) {
            if (curr->val_ == key) {
                return curr;
            } else if (Compare{}(key, curr->val_)) {
                curr = curr->left_;
            } else {
                curr = curr->right_;
            }
        }

        return nullptr;
    }

private:
    void Swap(BST& other) {
        std::swap(root_, other.root_);
        std::swap(size_, other.size_);
    }

    void Transplant(nptr old_root, nptr new_root) {
        if (old_root->parent_ == nullptr) {
            root_ = new_root;
        } else if (old_root == old_root->parent_->left_) {
            old_root->parent_->left_ = new_root;
        } else {
            old_root->parent_->right_ = new_root;
        }

        if (new_root != nullptr) {
            new_root->parent_ = old_root->parent_;
        }
    }
};

#endif //BST_H
