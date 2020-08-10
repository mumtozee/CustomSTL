#ifndef TREAP_H
#define TREAP_H

#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <stack>

#define INF std::numeric_limits<int>::max()

int GeneratePriority() {
    const int kSeed = std::numeric_limits<int>::max() - 1;
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<> distr(-kSeed, kSeed);

    return distr(generator);
}

template <class T>
struct TreapNode {
    T val_;
    int prior_;
    size_t size_;
    TreapNode* left_;
    TreapNode* right_;
    TreapNode* parent_;

    TreapNode() = default;

    explicit TreapNode(const T& data,
                       int prior = INF,
                       size_t size = 1,
                       TreapNode* left = nullptr,
                       TreapNode* right = nullptr,
                       TreapNode* parent = nullptr)
            : val_(data),
              prior_(prior),
              size_(size),
              left_(left),
              right_(right),
              parent_(parent) {}

    TreapNode(const TreapNode& other) = default;

    TreapNode(TreapNode&& other) noexcept
            : val_(other.val_),
              prior_(other.prior_),
              size_(other.size_),
              left_(other.left_),
              right_(other.right_),
              parent_(other.parent_) {
        other.val_ = T();
        other.prior_ = INF;
        other.size_ = 0;
        other.left_ = nullptr;
        other.right_ = nullptr;
        other.parent_ = nullptr;
    }

    TreapNode& operator=(const TreapNode& other) {
        TreapNode(other).Swap(*this);
        return *this;
    }

    TreapNode& operator=(TreapNode&& other) {
        Swap(other);
        return *this;
    }

    virtual ~TreapNode() = default;

private:
    virtual void Swap(TreapNode& other) {
        std::swap(val_, other.val_);
        std::swap(prior_, other.prior_);
        std::swap(size_, other.size_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(parent_, other.parent_);
    }
};

template <
        class T,
        class Compare = std::less<>
>
class Treap {
    using nptr = TreapNode<T>*;

public:
    template <bool isConst = false>
    class Iterator {
    public:
        using value_type = T;
        using reference = typename std::conditional<isConst, const T&, T&>::type;
        using pointer = typename std::conditional<isConst, const T*, T*>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t;

        explicit Iterator(const Treap& tree, nptr point = nullptr, nptr root = nullptr)
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
        const Treap& outer_;
    };

    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    /* constructor definitions */
    Treap() : root_(nullptr) {
    }

    Treap(std::initializer_list<T> init) : Treap() {
        for (const T& item : init) {
            Insert(item);
        }
    }

    Treap(const Treap& other) {
        nptr other_root = other.root_;
        root_ = new TreapNode<T>(other_root->val_, other_root->prior_, other_root->size_);
        nptr new_root = root_;

        while (other_root != nullptr) {
            if (other_root->right_ != nullptr && new_root->right_ == nullptr) {
                new_root->right_ = new TreapNode<T>(other_root->right_->val_,
                                                  other_root->right_->prior_,
                                                  other_root->right_->size_,
                                                  nullptr, nullptr, new_root);
                new_root = new_root->right_;
                other_root = other_root->right_;
            } else if (other_root->left_ != nullptr && new_root->left_ == nullptr) {
                new_root->left_ = new TreapNode<T>(other_root->left_->val_,
                                                 other_root->left_->prior_,
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

    Treap(Treap&& other) noexcept: root_(other.root_) {
        other.root_ = nullptr;
    }

    Treap& operator=(const Treap& other) {
        Treap(other).Swap(*this);
        return *this;
    }

    Treap& operator=(Treap&& other) noexcept {
        Swap(other);
        return *this;
    }

    ~Treap() {
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
        auto rand_prio = GeneratePriority();
        nptr new_node = new TreapNode<T>(item, rand_prio, 1, nullptr, nullptr, nullptr);
        Add(new_node);
    }

    void Delete(const T& item) {
        nptr node = Search(item);
        if (node == nullptr) {
            return; // the item was not found in the tree
        }
        Remove(node);
    }

    void Swap(Treap& other) {
        std::swap(root_, other.root_);
    }

    std::pair<Treap, Treap> Split(const T& item) {
        nptr pivot = new TreapNode<T>(item, root_->prior_, 1, nullptr, nullptr, nullptr);
        Add(pivot);
        nptr left_tree_root = pivot->left_;
        nptr right_tree_root = pivot->right_;
        pivot->left_ = nullptr;
        pivot->right_ = nullptr;
        return std::make_pair<Treap, Treap>({left_tree_root}, {right_tree_root});
    }

protected:
    nptr root_;

    explicit Treap(nptr root) : root_(root) {
        root_->parent_ = nullptr;
    }

private:
    void Add(nptr new_node) {
        nptr curr = root_;
        nptr parent = nullptr;
        while (curr != nullptr) {
            parent = curr;
            if (Compare{}(new_node->val_, curr->val_)) {
                curr = curr->left_;
            } else {
                curr = curr->right_;
            }
        }

        new_node->parent_ = parent;
        if (parent == nullptr) {
            root_ = new_node;
        } else if (Compare{}(new_node->val_, parent->val_)) {
            new_node->parent_->left_ = new_node;
        } else {
            new_node->parent_->right_ = new_node;
        }
        UpdateSize(new_node);
        FixInsert(new_node);
    }

    void Remove(nptr node) {
        while (node->left_ != nullptr || node->right_ != nullptr) {
            if (node->left_ == nullptr) {
                Transplant(node, node->right_);
                break;
            } else if (node->right_ == nullptr) {
                Transplant(node, node->left_);
                break;
            } else {
                if (node->left_->prior_ > node->right_->prior_) {
                    RightRotate(node);
                } else {
                    LeftRotate(node);
                }
            }
        }

        nptr fix_node = node->parent_;
        if (fix_node != nullptr && node == fix_node->left_) {
            fix_node->left_ = nullptr;
        } else if (fix_node != nullptr && node == fix_node->right_) {
            fix_node->right_ = nullptr;
        }
        delete node;
        UpdateSize(fix_node);
    }

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

    size_t Size(nptr node) const {
        return (node == nullptr) ? 0 : node->size_;
    }

    void UpdateSize(nptr node) {
        nptr cur = node;
        while (cur != nullptr) {
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

    void FixInsert(nptr node) {
        nptr curr = node;
        nptr fix_node = nullptr;
        while (curr != nullptr && curr->parent_ != nullptr) {
            if (curr->parent_->prior_ <= curr->prior_) {
                fix_node = curr->parent_;
                if (curr == curr->parent_->left_) {
                    RightRotate(curr->parent_);
                } else {
                    LeftRotate(curr->parent_);
                }
                UpdateSize(fix_node);
            } else {
                break;
            }
        }
    }

    nptr Search(const T& item) const {
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

    bool isLeaf(nptr node) const {
        return (node->left_ == nullptr && node->right_ == nullptr);
    }

    bool isLeftChild(nptr node) const {
        if (node->parent_ == nullptr) {
            return false;
        }
        return node == node->parent_->left_;
    }

    bool isRightChild(nptr node) const {
        if (node->parent_ == nullptr) {
            return false;
        }
        return node == node->parent_->right_;
    }
};

template <class T>
Treap<T> Merge(Treap<T>& left_tree, Treap<T>& right_tree) {
    TreapNode<T>* left_root = left_tree.root_;
    TreapNode<T>* right_root = right_tree.root_;

    if (left_root == nullptr) {
        return {right_root};
    } else if (right_root == nullptr) {
        return {left_root};
    }

    int dummy_prio = std::max(left_root->prior_, right_root->prior_);
    size_t dummy_size = Size(left_root) + Size(right_root) + 1;
    TreapNode<T>* dummy_root_ = new TreapNode<T>(T(), dummy_prio, dummy_size, left_root, right_root, nullptr);
    left_root->parent_ = dummy_root_;
    right_root->parent_ = dummy_root_;
    Remove(dummy_root_);

    if (left_root->prior_ == dummy_prio) {
        return {left_root};
    } else {
        return {right_root};
    }
}

#endif //TREAP_H
