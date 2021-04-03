#pragma once
#include <deque>
#include <thread>
#include <vector>
#include <mutex>
#include <unordered_map>
#include "ThreadTask.h"

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    void AddTask(const ThreadTask task);
    void ClearTasks();
    void SearchThread();
    bool IsFinished();
private:
    std::deque<ThreadTask> tasksQueue;
    std::vector<std::thread> threads;
    std::mutex mutex;
    std::unordered_map<std::thread::id, bool> threadStatus;
};