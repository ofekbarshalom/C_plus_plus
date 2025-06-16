#ifndef ACTIVE_OBJECT_HPP
#define ACTIVE_OBJECT_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <memory>
#include "../core/Graph.hpp"

using namespace std;
using namespace GraphAlgo;

// Struct to carry client request through the pipeline
struct Request {
    Graph graph;
    int clientSocket;
    string mstResult, sccResult, cliqueResult, maxFlowResult;
};

// Generic Active Object class
template<typename T>
class ActiveObject {
    queue<T> q;
    mutex mtx;
    condition_variable cv;
    thread worker;
    function<void(T)> task;
    bool stop = false;

public:
    ActiveObject(function<void(T)> taskFn) : task(taskFn) {
        worker = thread([this]() { run(); });
    }

    void enqueue(T item) {
        unique_lock<mutex> lock(mtx);
        q.push(item);
        cv.notify_one();
    }

    void run() {
        while (true) {
            T item;
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this]() { return stop || !q.empty(); });
                if (stop && q.empty()) return;
                item = q.front();
                q.pop();
            }
            task(item);
        }
    }

    void shutdown() {
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        if (worker.joinable()) worker.join();
    }

    ~ActiveObject() {
        shutdown();
    }
};

#endif // ACTIVE_OBJECT_HPP
