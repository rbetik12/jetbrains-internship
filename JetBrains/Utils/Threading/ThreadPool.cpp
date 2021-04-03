#include "ThreadPool.h"
#include <sstream>
#include <string>
#include "../Search/Search.h"

ThreadPool::ThreadPool() {
    uint32_t threadsAmount = std::thread::hardware_concurrency();
    threads.reserve(threadsAmount);
    for (uint32_t i = 0; i < threadsAmount; i++) {
        std::thread thread(&ThreadPool::SearchThread, this);
        threads.push_back(std::move(thread));
        threadStatus[thread.get_id()] = false;
    }
}

ThreadPool::~ThreadPool() {}

void ThreadPool::ClearTasks() {
    std::lock_guard<std::mutex> guard(mutex);
    tasksQueue.clear();
}

void ThreadPool::AddTask(const ThreadTask task) {
    std::lock_guard<std::mutex> guard(mutex);
    tasksQueue.push_back(task);
}

void ThreadPool::SearchThread() {
    while (true) {
        ThreadTask task;
        {
            std::lock_guard<std::mutex> guard(mutex);
            if (tasksQueue.empty()) {
                threadStatus[std::this_thread::get_id()] = false;
                continue;
            }
            threadStatus[std::this_thread::get_id()] = true;
            task = tasksQueue.front();
            tasksQueue.pop_front();
        }

        for (int i = task.bytesToSearch; i >= 0; i--) {
            if (task.block[i] == '\n') break;
            else task.block[i] = '\0';
        }

        std::stringstream ss(task.block);
        std::string dictString;
        while (std::getline(ss, dictString, '\n')) {
            if (Contains(dictString, *task.stringToSearch)) {
                {
                    std::lock_guard<std::mutex> guard(mutex);
                    task.results->push_back(dictString);
                }
            }
        }
        delete[] task.block;
    }
}

bool ThreadPool::IsFinished() {
    for (auto it = threadStatus.begin(); it != threadStatus.end(); it++) {
        if (it->second) {
            return false;
        }
    }

    return true;
}
