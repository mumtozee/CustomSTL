#ifndef STACK_QUEUE_STACK_H
#define STACK_QUEUE_STACK_H

#include <cctype>

struct Node;

class Stack {
public:
    void Push(int value);
    void Pop();
    void Clear();
    int Top() const;
    int& Top();
    bool Empty() const;
    size_t Size() const;

    Stack();
    Stack(const Stack &other);
    Stack& operator=(Stack other);
    ~Stack();

    void Swap(Stack& other);
private:
    Node* top_;
    size_t size_;

    void Reverse();
};

#endif //STACK_QUEUE_STACK_H
