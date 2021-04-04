#include "ThreadPool.h"
#include <sstream>
#include <string>
#include "../Search/Search.h"
#include <iostream>

ThreadPool::ThreadPool() {
    uint32_t threadsAmount = 1;
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
    std::cout << "Added block " << task.blockID << std::endl;
}

void ThreadPool::SearchThread() {
    while (true) {
        ThreadTask task;
        {
            if (tasksQueue.empty()) {
                threadStatus[std::this_thread::get_id()] = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                continue;
            }
            threadStatus[std::this_thread::get_id()] = true;
            std::lock_guard<std::mutex> guard(mutex);
            // This additional check guarantees that queue is not empty
            if (tasksQueue.empty()) {
                continue;
            }
            task = tasksQueue.front();
            tasksQueue.pop_front();
            std::cout << "Thread " << std::this_thread::get_id() << " picked block " << task.blockID << std::endl;
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

        //std::cout << "Thread " << std::this_thread::get_id() << " finished searching in block " << task.blockID << std::endl;
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
