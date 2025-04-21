// ofekbarshalom@gmail.com

#ifndef QUEUE_HPP
#define QUEUE_HPP

const int MAX_SIZE = 100;

struct Queue {
    int data[MAX_SIZE];
    int head;
    int tail;
    int size;

    Queue();   // Constructor
    ~Queue();  // Destructor

    bool isEmpty() const;
    void enqueue(int value);
    int dequeue();
};

#endif // QUEUE_HPP
