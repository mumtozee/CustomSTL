#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <functional>
#include <iostream>
#include <stack>

#define RED false
#define BLACK true

template <class T>
struct RBNode {
    T val_;
    RBNode* left_;
    RBNode* right_;
    RBNode* parent_;
    bool color_;

    RBNode() = default;

    explicit constexpr RBNode(const T& data,
                              RBNode* left = nullptr,
                              RBNode* right = nullptr,
                              RBNode* parent = nullptr,
                              bool col = RED)
            : val_(data),
              left_(left),
              right_(right),
              parent_(parent),
              color_(col) {}

    RBNode(const RBNode& other) = default;

    RBNode(RBNode&& other) noexcept
            : val_(other.val_),
              left_(other.left_),
              right_(other.right_),
              parent_(other.parent_),
              color_(other.color_) {}

    RBNode& operator=(const RBNode& other) {
        RBNode(other).Swap(*this);
        return *this;
    }

    RBNode& operator=(RBNode&& other) {
        Swap(other);
        return *this;
    }

    ~RBNode() = default;

private:
    void Swap(RBNode& other) {
        std::swap(val_, other.val_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(parent_, other.parent_);
        std::swap(color_, other.color_);
    }
};

template <
        class T,
        class Compare = std::less<T>
>
class RedBlackTree {
    using nptr = RBNode<T>*;

public:
    template <bool isConst = false>
    class Iterator {
    public:
        using value_type = T;
        using reference = typename std::conditional<isConst, const T&, T&>::type;
        using pointer = typename std::conditional<isConst, const T*, T*>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t;

        explicit Iterator(const RedBlackTree& tree, nptr point = nullptr, nptr root = nullptr)
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
            if (current_ == outer_.null_) {
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
        const RedBlackTree& outer_;
    };

    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    /* constructor definitions */
    RedBlackTree() : size_(0) {
        null_ = new RBNode<T>(T(), nullptr, nullptr, nullptr, BLACK);
        root_ = null_;
    }

    RedBlackTree(std::initializer_list<T> init) : RedBlackTree() {
        for (const T& item : init) {
            Insert(item);
        }
    }

    RedBlackTree(const RedBlackTree& other) : size_(other.size_) {
        null_ = new RBNode<T>(T(), nullptr, nullptr, nullptr, BLACK);
        if (other.root_ == other.null_) {
            root_ = null_;
            return;
        }

        nptr other_root = other.root_;
        root_ = new RBNode<T>(other_root->val_, null_, null_, null_, other_root->color_);
        nptr new_root = root_;

        while (other_root != other.null_) {
            if (other_root->right_ != other.null_ && new_root->right_ == null_) {
                new_root->right_ = new RBNode<T>(other_root->right_->val_, null_, null_, new_root, other_root->color_);
                new_root = new_root->right_;
                other_root = other_root->right_;
            } else if (other_root->left_ != other.null_ && new_root->left_ == null_) {
                new_root->left_ = new RBNode<T>(other_root->left_->val_, null_, null_, new_root, other_root->color_);
                new_root = new_root->left_;
                other_root = other_root->left_;
            } else {
                new_root = new_root->parent_;
                other_root = other_root->parent_;
            }
        }
    }

    RedBlackTree(RedBlackTree&& other) noexcept: size_(other.size_), null_(other.null_), root_(other.root_) {
        other.size_ = 0;
        other.root_ = nullptr;
        other.null_ = nullptr;
    }

    RedBlackTree& operator=(const RedBlackTree& other) {
        RedBlackTree(other).Swap(*this);
        return *this;
    }

    RedBlackTree& operator=(RedBlackTree&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~RedBlackTree() {
        Clear();
        delete null_;
    }

    iterator begin() {
        return iterator(*this, GetMinimum(), root_);
    }

    const_iterator cbegin() const {
        return const_iterator(*this, GetMinimum(), root_);
    }

    iterator end() {
        nptr last = GetMaximum();
        if (last == null_) {
            return iterator(*this, last, root_);
        }
        return iterator(*this, last->right_, root_);
    }

    const_iterator cend() const {
        nptr last = GetMaximum();
        if (last == null_) {
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

    iterator Find(const_reference item) {
        auto pos = Search(item);
        if (pos == null_) {
            return end();
        }
        return iterator(*this, pos, root_);
    }

    const_iterator Find(const_reference item) const {
        auto pos = Search(item);
        if (pos == null_) {
            return cend();
        }
        return const_iterator(*this, pos, root_);
    }

    void Print() const {
        nptr curr = root_;
        std::stack<nptr> visited;

        while (curr != null_ || !visited.empty()) {
            while (curr != null_) {
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
        if (root_ == null_) {
            return;
        }

        auto curr = GetMinimum();
        while (curr != null_ && !isLeaf(root_)) {
            if (isLeaf(curr) && isLeftChild(curr)) {
                auto succ = LeafSuccessor(curr);
                delete curr;
                succ->left_ = null_;
                curr = succ;
            } else if (isLeaf(curr) && isRightChild(curr)) {
                auto pre = LeafPredecessor(curr);
                delete curr;
                pre->right_ = null_;
                curr = pre;
            } else if (!isLeaf(curr)) {
                curr = LeafSuccessor(curr);
            }
        }
        delete root_;
        root_ = null_;
        size_ = 0;
    }

    void Insert(const T& item) {
        nptr new_node = new RBNode<T>(item, null_, null_, null_, RED);

        nptr curr = root_;
        nptr tmp_parent = null_;

        while (curr != null_) {
            tmp_parent = curr;
            if (Compare{}(item, curr->val_)) {
                curr = curr->left_;
            } else {
                curr = curr->right_;
            }
        }

        new_node->parent_ = tmp_parent;
        if (tmp_parent == null_) {
            root_ = new_node;
        } else if (Compare{}(item, tmp_parent->val_)) {
            new_node->parent_->left_ = new_node;
        } else {
            new_node->parent_->right_ = new_node;
        }
        ++size_;
        RB_Insert_Fixup(new_node);
    }

    void Delete(const T& item) {
        nptr node = Search(item);
        if (node == null_) {
            return;
        }

        nptr original_node = node;
        bool original_color = node->color_;
        nptr tmp = null_;

        if (node->right_ == null_) {
            tmp = node->left_;
            RB_Transplant(node, node->left_);
        } else if (node->left_ == nullptr) {
            tmp = node->right_;
            RB_Transplant(node, node->right_);
        } else {
            original_node = SubtreeMinimum(node->right_);
            original_color = original_node->color_;
            tmp = original_node->right_;
            if (original_node->parent_ == node) {
                tmp->parent_ = original_node;
            } else {
                RB_Transplant(original_node, original_node->right_);
                original_node->right_ = node->right_;
                original_node->right_->parent_ = original_node;
            }

            RB_Transplant(node, original_node);
            original_node->left_ = node->left_;
            original_node->left_->parent_ = original_node;
            original_node->color_ = node->color_;
        }

        delete node;
        node = null_;
        --size_;

        if (original_color == BLACK) {
            RB_Delete_Fixup(tmp);
        }
    }

    void Swap(RedBlackTree& other) {
        std::swap(size_, other.size_);
        std::swap(null_, other.null_);
        std::swap(root_, other.root_);
    }

protected:
    size_type size_;
    nptr null_;
    nptr root_;

private:
    nptr SubtreeMinimum(nptr root) const {
        auto curr = root;
        while (curr != null_ && curr->left_ != null_) {
            curr = curr->left_;
        }
        return curr;
    }

    nptr SubtreeMaximum(nptr root) const {
        auto curr = root;
        while (curr != null_ && curr->right_ != null_) {
            curr = curr->right_;
        }
        return curr;
    }

    nptr LeafSuccessor(nptr node) const {
        nptr curr = node;
        if (curr->right_ != null_) {
            return SubtreeMinimum(curr->right_);
        } else {
            while (curr->parent_ != null_ && curr == curr->parent_->right_) {
                curr = curr->parent_;
            }
        }
        return curr->parent_;
    }

    nptr LeafPredecessor(nptr node) const {
        nptr curr = node;
        if (curr->left_ != null_) {
            return SubtreeMaximum(curr->left_);
        } else {
            while (curr->parent_ != null_ && curr == curr->parent_->left_) {
                curr = curr->parent_;
            }
        }
        return curr->parent_;
    }

    void LeftRotate(nptr node) {
        nptr tmp_node = node->right_;
        node->right_ = tmp_node->left_;

        if (tmp_node->left_ != null_) {
            tmp_node->left_->parent_ = node;
        }

        tmp_node->parent_ = node->parent_;

        if (node->parent_ == null_) {
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

        if (beta_node->right_ != null_) {
            beta_node->right_->parent_ = node;
        }

        beta_node->parent_ = node->parent_;

        if (node->parent_ == null_) {
            root_ = beta_node;
        } else if (node == node->parent_->left_) {
            node->parent_->left_ = beta_node;
        } else {
            node->parent_->right_ = beta_node;
        }

        beta_node->right_ = node;
        node->parent_ = beta_node;
    }

    nptr Search(const T& item) {
        nptr curr = root_;

        while (curr != null_) {
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

    void RB_Insert_Fixup(nptr node) {
        while (node->parent_->color_ == RED) {
            nptr grandpa = node->parent_->parent_;

            if (node->parent_ == grandpa->left_) {
                nptr uncle = grandpa->right_;

                if (uncle->color_ == RED) {
                    node->parent_->color_ = BLACK;
                    uncle->color_ = BLACK;
                    grandpa->color_ = RED;
                    node = grandpa;
                } else {
                    if (node == node->parent_->right_) {
                        node = node->parent_;
                        LeftRotate(node);
                    }

                    node->parent_->color_ = BLACK;
                    node->parent_->parent_->color_ = RED;
                    RightRotate(node->parent_->parent_);
                }
            } else {
                nptr uncle = grandpa->left_;

                if (uncle->color_ == RED) {
                    node->parent_->color_ = BLACK;
                    uncle->color_ = BLACK;
                    grandpa->color_ = RED;
                    node = grandpa;
                } else {
                    if (node == node->parent_->left_) {
                        node = node->parent_;
                        RightRotate(node);
                    }

                    node->parent_->color_ = BLACK;
                    node->parent_->parent_->color_ = RED;
                    LeftRotate(node->parent_->parent_);
                }
            }
        }

        root_->color_ = BLACK;
    }

    void RB_Transplant(nptr node, nptr new_node) {
        if (node->parent_ == null_) {
            root_ = new_node;
        } else if (node == node->parent_->left_) {
            node->parent_->left_ = new_node;
        } else {
            node->parent_->right_ = new_node;
        }

        new_node->parent_ = node->parent_;
    }

    void RB_Delete_Fixup(nptr node) {
        while (node != root_ && node->color_ == BLACK) {
            if (node == node->parent_->left_) {
                nptr sibling = node->parent_->right_;
                if (sibling->color_ == RED) {
                    sibling->color_ = BLACK;
                    node->parent_->color_ = RED;
                    LeftRotate(node->parent_);
                    sibling = node->parent_->right_;
                }

                if (sibling->left_->color_ == BLACK && sibling->right_->color_ == BLACK) {
                    sibling->color_ = RED;
                    node = node->parent_;
                } else {
                    if (sibling->right_->color_ == BLACK) {
                        sibling->left_->color_ = BLACK;
                        sibling->color_ = RED;
                        RightRotate(sibling);
                        sibling = node->parent_->right_;
                    }

                    sibling->color_ = node->parent_->color_;
                    node->parent_->color_ = BLACK;
                    sibling->right_->color_ = BLACK;
                    LeftRotate(node->parent_);
                    node = root_;
                }
            } else {
                nptr sibling = node->parent_->left_;
                if (sibling->color_ == RED) {
                    sibling->color_ = BLACK;
                    node->parent_->color_ = RED;
                    RightRotate(node->parent_);
                    sibling = node->parent_->left_;
                }

                if (sibling->right_->color_ == BLACK && sibling->left_->color_ == BLACK) {
                    sibling->color_ = RED;
                    node = node->parent_;
                } else {
                    if (sibling->left_->color_ == BLACK) {
                        sibling->right_->color_ = BLACK;
                        sibling->color_ = RED;
                        LeftRotate(sibling);
                        sibling = node->parent_->left_;
                    }

                    sibling->color_ = node->parent_->color_;
                    node->parent_->color_ = BLACK;
                    sibling->left_->color_ = BLACK;
                    RightRotate(node->parent_);
                    node = root_;
                }
            }
        }

        node->color_ = BLACK;
    }

    bool isLeaf(nptr node) {
        return (node->left_ == null_ && node->right_ == null_);
    }

    bool isLeftChild(nptr node) {
        if (node->parent_ == null_) {
            return false;
        }

        return node == node->parent_->left_;
    }

    bool isRightChild(nptr node) {
        if (node->parent_ == null_) {
            return false;
        }

        return node == node->parent_->right_;
    }
};

#endif //REDBLACKTREE_H
