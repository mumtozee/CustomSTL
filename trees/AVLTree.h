#ifndef AVLTREE_H
#define AVLTREE_H

#include <cctype>
#include <functional>
#include <iostream>
#include <stack>
#include <utility>

template <class T>
struct AVLNode {
    T val_;
    size_t height_;
    size_t size_; // subtree size of the node
    AVLNode* left_;
    AVLNode* right_;
    AVLNode* parent_;

    AVLNode() = default;

    explicit AVLNode(const T& data,
                     size_t height = 1,
                     size_t size = 1,
                     AVLNode* left = nullptr,
                     AVLNode* right = nullptr,
                     AVLNode* parent = nullptr)
            : val_(data),
              height_(height),
              size_(size),
              left_(left),
              right_(right),
              parent_(parent) {}

    AVLNode(const AVLNode& other) = default;

    AVLNode(AVLNode&& other) noexcept
            : val_(other.val_),
              height_(other.height_),
              size_(other.size_),
              left_(other.left_),
              right_(other.right_),
              parent_(other.parent_) {}

    AVLNode& operator=(const AVLNode& other) {
        Node(other).Swap(*this);
        return *this;
    }

    AVLNode& operator=(AVLNode&& other) {
        Swap(other);
        return *this;
    }

    virtual ~AVLNode() = default;

private:
    virtual void Swap(AVLNode& other) {
        std::swap(val_, other.val_);
        std::swap(height_, other.height_);
        std::swap(size_, other.size_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(parent_, other.parent_);
    }
};

template <
        class T,
        class Compare = std::less<T>
>
class AVLTree {
    using nptr = AVLNode<T>*;

public:
    template <bool isConst = false>
    class Iterator {
    public:
        using value_type = T;
        using reference = typename std::conditional<isConst, const T&, T&>::type;
        using pointer = typename std::conditional<isConst, const T*, T*>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t;

        explicit Iterator(const AVLTree& tree, nptr point = nullptr, nptr root = nullptr)
                : current_(point),
                  root_node_(root),
                  outer_(tree) {}

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
            current_ = outer_.LeafSuccessor(current_);
            return *this;
        }

        Iterator& operator--() {
            if (current_ == nullptr) {
                current_ = outer_.SubtreeMaximum(root_node_);
                return *this;
            }

            current_ = outer_.LeafPredecessor(current_);

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
        const AVLTree& outer_;
    };

    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    /* constructor definitions */
    AVLTree() : root_(nullptr) {
    }

    AVLTree(std::initializer_list<T> init) : AVLTree() {
        for (const T& item : init) {
            Insert(item);
        }
    }

    AVLTree(const AVLTree& other) {
        nptr other_root = other.root_;
        root_ = new AVLNode<T>(other_root->val_, other_root->height_, other.size_);
        nptr new_root = root_;

        while (other_root != nullptr) {
            if (other_root->right_ != nullptr && new_root->right_ == nullptr) {
                new_root->right_ = new AVLNode<T>(other_root->right_->val_,
                                                  other_root->right_->height_,
                                                  other_root->right_->size_,
                                                  nullptr, nullptr, new_root);
                new_root = new_root->right_;
                other_root = other_root->right_;
            } else if (other_root->left_ != nullptr && new_root->left_ == nullptr) {
                new_root->left_ = new AVLNode<T>(other_root->left_->val_,
                                                 other_root->left_->height_,
                                                 other_root->left_->size_,
                                                 nullptr, nullptr, new_root);
                new_root = new_root->left_;
                other_root = other_root->left_;
            } else {
                new_root = new_root->parent_;
                other_root = other_root->parent_;
            }
        }
    }

    AVLTree(AVLTree&& other) noexcept: root_(other.root_) {
        other.root_ = nullptr;
    }

    AVLTree& operator=(const AVLTree& other) {
        AVLTree(other).Swap(*this);
        return *this;
    }

    AVLTree& operator=(AVLTree&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~AVLTree() {
        Clear();
    }

    iterator begin() {
        return iterator(*this, GetMinimum(), root_);
    }

    const_iterator cbegin() const {
        return const_iterator(*this, GetMinimum(), root_);
    }

    iterator end() {
        nptr last = GetMaximum();
        if (last == nullptr) {
            return iterator(*this, last, root_);
        }
        return iterator(*this, last->right_, root_);
    }

    const_iterator cend() const {
        nptr last = GetMaximum();
        if (last == nullptr) {
            return const_iterator(*this, last, root_);
        }
        return const_iterator(*this, last->right_, root_);
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

    size_t Rank(const T& item) const {
        nptr node = Search(item);
        size_t rights = Size(node->left_);
        nptr curr = node;

        while (curr != root_) {
            if (curr == curr->parent_->right_) {
                rights += Size(curr->parent_->left_) + 1;
            }

            curr = curr->parent_;
        }

        return rights;
    }

    T Select(size_t idx) const {
        return Select(idx, root_);
    }

    size_type Size() const {
        return Size(root_);
    }

    iterator Find(const_reference item) {
        auto pos = Search(item);
        if (pos == nullptr) {
            return end();
        }
        return iterator(*this, pos, root_);
    }

    const_iterator Find(const_reference item) const {
        auto pos = Search(item);
        if (pos == nullptr) {
            return cend();
        }
        return const_iterator(*this, pos, root_);
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
    }

    void Insert(const T& item) {
        nptr new_node = new AVLNode<T>(item, 1, 1, nullptr, nullptr, nullptr);

        nptr parent = nullptr;
        nptr curr = root_;

        while (curr != nullptr) {
            parent = curr;

            if (Compare{}(item, curr->val_)) {
                curr = curr->left_;
            } else if (Compare{}(curr->val_, item)) {
                curr = curr->right_;
            } else {
                return;
            }
        }

        new_node->parent_ = parent;
        if (parent == nullptr) {
            root_ = new_node;
        } else if (Compare{}(item, parent->val_)) {
            new_node->parent_->left_ = new_node;
        } else {
            new_node->parent_->right_ = new_node;
        }

        UpdateHeightAndSize(parent);
        FixInsert(parent);
    }

    void Delete(const T& item) {
        nptr to_delete_node = Search(item);

        if (to_delete_node == nullptr) {
            return; // the item was not found in the tree
        }

        nptr fix_node = to_delete_node->parent_;

        if (to_delete_node->left_ == nullptr) {
            Transplant(to_delete_node, to_delete_node->right_);
        } else if (to_delete_node->right_ == nullptr) {
            Transplant(to_delete_node, to_delete_node->left_);
        } else {
            nptr succ = SubtreeMinimum(to_delete_node->right_);
            if (to_delete_node != succ->parent_) {
                fix_node = succ->parent_;
                Transplant(succ, succ->right_);
                succ->right_ = to_delete_node->right_;
                succ->right_->parent_ = succ;
            } else {
                fix_node = succ;
            }

            Transplant(to_delete_node, succ);
            succ->left_ = to_delete_node->left_;
            succ->left_->parent_ = succ;
        }
        UpdateHeightAndSize(fix_node);
        FixDelete(fix_node);
    }

    void Swap(AVLTree& other) {
        std::swap(root_, other.root_);
    }

    int8_t BalanceFactor() const {
        return getBalanceFactor(root_);
    }

protected:
    nptr root_;

private:
    T Select(size_t idx, nptr node) const {
        size_t lefts = Size(node->left_);
        if (lefts == idx) {
            return node->val_;
        } else if (idx < lefts) {
            return Select(idx, node->left_);
        } else {
            return Select(idx - (lefts + 1), node->right_);
        }
    }

    nptr SubtreeMinimum(nptr root) const {
        auto curr = root;
        while (curr != nullptr && curr->left_ != nullptr) {
            curr = curr->left_;
        }
        return curr;
    }

    nptr SubtreeMaximum(nptr root) const {
        auto curr = root;
        while (curr != nullptr && curr->right_ != nullptr) {
            curr = curr->right_;
        }
        return curr;
    }

    nptr LeafSuccessor(nptr node) const {
        nptr curr = node;
        if (curr->right_ != nullptr) {
            return SubtreeMinimum(curr->right_);
        } else {
            while (curr->parent_ != nullptr && curr == curr->parent_->right_) {
                curr = curr->parent_;
            }
        }
        return curr->parent_;
    }

    nptr LeafPredecessor(nptr node) const {
        nptr curr = node;
        if (curr->left_ != nullptr) {
            return SubtreeMaximum(curr->left_);
        } else {
            while (curr->parent_ != nullptr && curr == curr->parent_->left_) {
                curr = curr->parent_;
            }
        }
        return curr->parent_;
    }

    size_t Height(nptr node) const {
        return (node == nullptr) ? 0 : node->height_;
    }

    size_t Size(nptr node) const {
        return (node == nullptr) ? 0 : node->size_;
    }

    void UpdateSingleNode(nptr node) {
        if (node == nullptr) {
            return;
        }
        node->height_ = std::max(Height(node->left_), Height(node->right_)) + 1;
        node->size_ = Size(node->left_) + Size(node->right_) + 1;
    }

    void UpdateHeightAndSize(nptr node) {
        nptr cur = node;
        while (cur != nullptr) {
            cur->height_ = std::max(Height(cur->left_), Height(cur->right_)) + 1;
            cur->size_ = Size(cur->left_) + Size(cur->right_) + 1;
            cur = cur->parent_;
        }
    }

    void Transplant(nptr old_node, nptr new_node) {
        if (old_node->parent_ == nullptr) {
            root_ = new_node;
        } else if (old_node == old_node->parent_->left_) {
            old_node->parent_->left_ = new_node;
        } else {
            old_node->parent_->right_ = new_node;
        }

        if (new_node != nullptr) {
            new_node->parent_ = old_node->parent_;
        }
    }

    void LeftRotate(nptr node) {
        nptr tmp_node = node->right_;
        node->right_ = tmp_node->left_;

        if (tmp_node->left_ != nullptr) {
            tmp_node->left_->parent_ = node;
        }

        tmp_node->parent_ = node->parent_;

        if (node->parent_ == nullptr) {
            root_ = tmp_node;
        } else if (node == node->parent_->left_) {
            node->parent_->left_ = tmp_node;
        } else {
            node->parent_->right_ = tmp_node;
        }

        tmp_node->left_ = node;
        node->parent_ = tmp_node;
    }

    void RightRotate(nptr node) {
        nptr beta_node = node->left_;

        node->left_ = beta_node->right_;

        if (beta_node->right_ != nullptr) {
            beta_node->right_->parent_ = node;
        }

        beta_node->parent_ = node->parent_;

        if (node->parent_ == nullptr) {
            root_ = beta_node;
        } else if (node == node->parent_->left_) {
            node->parent_->left_ = beta_node;
        } else {
            node->parent_->right_ = beta_node;
        }

        beta_node->right_ = node;
        node->parent_ = beta_node;
    }

    void BigLeftRotate(nptr node) {
        if (node->right_ == nullptr) {
            return;
        }
        RightRotate(node->right_);
        LeftRotate(node);
    }

    void BigRightRotate(nptr node) {
        if (node->left_ == nullptr) {
            return;
        }
        LeftRotate(node->left_);
        RightRotate(node);
    }

    int16_t getBalanceFactor(nptr root) const {
        return static_cast<int64_t>(Height(root->left_)) - static_cast<int64_t>(Height(root->right_));
    }

    void FixInsert(nptr node) {
        nptr curr = node;
        while (curr != nullptr) {
            auto balance = getBalanceFactor(curr);
            if (balance < -1 || balance > 1) { // till the first unbalanced node
                break;
            }
            curr = curr->parent_;
        }

        if (curr != nullptr) { // if the cycle was exited with break
            Rebalance(curr);
        }
    }

    void FixDelete(nptr node) {
        nptr curr = node;
        while (curr != nullptr) {
            auto balance = getBalanceFactor(curr);
            if (balance < -1 || balance > 1) { // re-balance all unbalanced nodes till the root
                curr = Rebalance(curr);
            }
            curr = curr->parent_;
        }
    }

    nptr Rebalance(nptr node) {
        auto balance = getBalanceFactor(node);
        nptr subtree_root = nullptr;
        if (balance > 1) {
            if (getBalanceFactor(node->left_) >= 0) {
                subtree_root = node->left_;
                RightRotate(node);
            } else {
                subtree_root = node->left_->right_;
                BigRightRotate(node);
            }
        } else if (balance < -1) {
            if (getBalanceFactor(node->right_) <= 0) {
                subtree_root = node->right_;
                LeftRotate(node);
            } else {
                subtree_root = node->right_->left_;
                BigLeftRotate(node);
            }
        }
        UpdateSingleNode(subtree_root->left_);
        UpdateSingleNode(subtree_root->right_);
        UpdateHeightAndSize(subtree_root);
        return subtree_root;
    }

    nptr Search(const T& item) {
        nptr curr = root_;

        while (curr != nullptr) {
            if (curr->val_ == item) {
                break;
            } else if (Compare{}(item, curr->val_)) {
                curr = curr->left_;
            } else {
                curr = curr->right_;
            }
        }

        return curr;
    }

    bool isLeaf(nptr node) {
        return (node->left_ == nullptr && node->right_ == nullptr);
    }

    bool isLeftChild(nptr node) {
        if (node->parent_ == nullptr) {
            return false;
        }
        return node == node->parent_->left_;
    }

    bool isRightChild(nptr node) {
        if (node->parent_ == nullptr) {
            return false;
        }
        return node == node->parent_->right_;
    }
};

#endif //AVLTREE_H
