#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class BlockingQueue {
private:
    queue<T> q;
    mutex mtx;
    condition_variable cv;

public:
    void push(const T& item) {
        unique_lock<mutex> lock(mtx);
        q.push(item);
        cv.notify_one();
    }

    T pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return !q.empty(); });
        T item = q.front();
        q.pop();
        return item;
    }
};

#endif
