//
// Created by MUMTOZBEK on 10.08.2020.
//

#include <cctype>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <stack>
#include <string>

#define INF std::numeric_limits<int>::max()

int RandomPriority() {
    const int kSeed = std::numeric_limits<int>::max() - 1;
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<> distr(-kSeed, kSeed);
    return distr(generator);
}

template <class T>
struct SuperNode {
    T value_;
    int priority_;
    size_t size_;
    SuperNode* left_;
    SuperNode* right_;

    explicit SuperNode(const T& item,
                       int priority = RandomPriority(),
                       size_t size = 1,
                       SuperNode* left = nullptr,
                       SuperNode* right = nullptr)
            : value_(item),
              priority_(priority),
              size_(size),
              left_(left),
              right_(right) {}
};

template <class T>
size_t GetSize(SuperNode<T>* node) {
    return (!node) ? 0 : node->size_;
}

template <class T>
void UpdateSize(SuperNode<T>* node) {
    if (!node) {
        return;
    }
    node->size_ = GetSize(node->left_) + GetSize(node->right_) + 1;
}

template <class T, class Compare = std::less<>>
class SuperVector {
    using nptr = SuperNode<T>*;
public:
    friend void Split(size_t pos, nptr& subtree, nptr& left_tree, nptr& right_tree);
    friend void Merge(nptr& left_tree, nptr& right_tree);

    SuperVector() : root_(nullptr) {
    }

    ~SuperVector() {
        Clear();
    }

    void InsertAt(size_t position, const T& item) {
        nptr tmp_right = nullptr;
        nptr tmp_left = nullptr;
        Split(position, root_, tmp_left, tmp_right);
        nptr new_node = new SuperNode<T>(item);
        nptr new_left = Merge(tmp_left, new_node);
        root_ = Merge(new_left, tmp_right);
    }

    void DeleteAt(size_t begin, size_t end) {
        nptr tmp_left = nullptr;
        nptr mid = nullptr;
        nptr to_delete = nullptr;
        nptr tmp_right = nullptr;
        Split(begin, root_, tmp_left, mid);
        Split(end - begin + 1, mid, to_delete, tmp_right);
        ClearSubtree(to_delete);
        root_ = Merge(tmp_left, tmp_right);
    }

    size_t Size() const {
        return GetSize<T>(root_);
    }

    void Clear() {
        ClearSubtree(root_);
    }

    T GetAt(size_t position) {
        return Select(position, root_);
    }

protected:
    nptr root_;
private:
    void ClearSubtree(nptr root) {
        std::stack<nptr> visited;
        nptr curr = root;
        while (curr || !visited.empty()) {
            if (curr) {
                visited.push(curr->right_);
                visited.push(curr->left_);
                delete curr;
            }
            curr = visited.top();
            visited.pop();
        }
    }

    T Select(size_t idx, nptr node) const {
        size_t curr_pos = GetSize(node->left_);
        if (idx == curr_pos) {
            return node->value_;
        } else if (idx < curr_pos) {
            return Select(idx, node->left_);
        } else {
            return Select(idx - curr_pos - 1, node->right_);
        }
    }
};

template <class T>
SuperNode<T>* Merge(SuperNode<T>*& left, SuperNode<T>*& right) {
    if (!left) {
        return right;
    } else if (!right) {
        return left;
    }

    SuperNode<T>* answer = nullptr;
    if (left->priority_ > right->priority_) {
        left->right_ = Merge(left->right_, right);
        answer = left;
    } else {
        right->left_ = Merge(left, right->left_);
        answer = right;
    }
    UpdateSize(answer);
    return answer;
}

template <class T>
void Split(size_t pos, SuperNode<T>*& subtree, SuperNode<T>*& left_tree, SuperNode<T>*& right_tree) {
    if (!subtree) {
        left_tree = nullptr;
        right_tree = nullptr;
        return;
    }
    size_t curr_pos = GetSize(subtree->left_);
    if (curr_pos < pos) {
        Split(pos - curr_pos - 1, subtree->right_, left_tree, right_tree);
        subtree->right_ = left_tree;
        left_tree = subtree;
        UpdateSize(left_tree);
    } else {
        Split(pos, subtree->left_, left_tree, right_tree);
        subtree->left_ = right_tree;
        right_tree = subtree;
        UpdateSize(right_tree);
    }
}

int main() {
    SuperVector<std::string> super_string_array;
    size_t N;
    std::cin >> N;
    char op;
    size_t index;
    for (size_t i = 0; i < N; ++i) {
        std::cin >> op >> index;
        if (op == '+') {
            std::string input;
            std::cin >> input;
            super_string_array.InsertAt(index, input);
        } else if (op == '-') {
            size_t end;
            std::cin >> end;
            super_string_array.DeleteAt(index, end);
        } else if (op == '?') {
            std::cout << super_string_array.GetAt(index) << '\n';
        }
    }
    return 0;
}
