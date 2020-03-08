#include "Stack.h"

#include <utility>

struct Node {
    int value_;
    Node* next_;
};

void Stack::Push(int value) {
    Node* newNode = new Node{value, top_};
    top_ = newNode;
    ++size_;
}

void Stack::Pop() {
    Node* tmpNode = top_;
    top_ = top_->next_;
    delete tmpNode;
    --size_;
}

void Stack::Clear() {
    while (size_ > 0) {
        Pop();
    }
}

int Stack::Top() const {
    return top_->value_;
}

int& Stack::Top() {
    return top_->value_;
}

size_t Stack::Size() const {
    return size_;
}

bool Stack::Empty() const {
    return size_ == 0;
}

Stack::Stack() : size_(0), top_(nullptr) {
}

void Stack::Swap(Stack& other) {
    std::swap(this->size_, other.size_);
    std::swap(this->top_, other.top_);
}

void Stack::Reverse() {
    Node* current = top_;
    Node* prev = nullptr;
    Node* next = nullptr;

    while (current != nullptr) {
        next = current->next_;
        current->next_ = prev;
        prev = current;
        current = next;
    }

    top_ = prev;
}

Stack::Stack(const Stack& other) : Stack() {
    Node* tmpNode = other.top_;

    while (tmpNode != nullptr) {
        Push(tmpNode->value_);
        tmpNode = tmpNode->next_;
    }

    Reverse();
}

Stack& Stack::operator=(Stack other) {
    Swap(other);
    return *this;
}

Stack::~Stack() {
    Clear();
}