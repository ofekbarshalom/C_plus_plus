// ofekbarshalom@gmail.com

#include "Queue.hpp"
#include <stdexcept>

using namespace std;

Queue::Queue() {
    head = 0;
    tail = 0;
    size = 0;
}

Queue::~Queue() {
    // No dynamic memory, nothing to free
}

bool Queue::isEmpty() const {
    return size == 0;
}

void Queue::enqueue(int value) {
    if (size == MAX_SIZE) {
        throw runtime_error("Queue is full");
    }
    data[tail] = value;
    tail = (tail + 1) % MAX_SIZE;
    ++size;
}

int Queue::dequeue() {
    if (isEmpty()) {
        throw runtime_error("Queue is empty");
    }
    int value = data[head];
    head = (head + 1) % MAX_SIZE;
    --size;
    return value;
}

