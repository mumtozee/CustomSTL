#ifndef STACK_QUEUE_QUEUE_H
#define STACK_QUEUE_QUEUE_H

#include "Stack.h"

class Queue {
public:
    void Push(int value);
    void Pop();
    size_t Size() const;
    bool Empty() const;
    int& Front();
    int Front() const;
private:
    mutable Stack inbox_;
    mutable Stack outbox_;

    void MoveFromInToOut() const;
};

#endif //STACK_QUEUE_QUEUE_H
