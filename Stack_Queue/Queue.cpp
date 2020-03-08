#include "Queue.h"

void Queue::MoveFromInToOut() const {
    if (outbox_.Empty()) {
        while (!inbox_.Empty()) {
            outbox_.Push(inbox_.Top());
            inbox_.Pop();
        }
    }
}

void Queue::Push(int value) {
    inbox_.Push(value);
}

void Queue::Pop() {
    MoveFromInToOut();
    outbox_.Pop();
}

int& Queue::Front() {
    MoveFromInToOut();
    return outbox_.Top();
}

int Queue::Front() const {
    MoveFromInToOut();
    return outbox_.Top();
}

size_t Queue::Size() const {
    return inbox_.Size() + outbox_.Size();
}

bool Queue::Empty() const {
    return inbox_.Empty() && outbox_.Empty();
}