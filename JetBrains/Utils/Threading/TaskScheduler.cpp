#include "TaskScheduler.h"
#include <chrono>
#include <thread>
#include <iostream>
#include "../Search/Search.h"

TaskScheduler::TaskScheduler(): clearTasks(false), searchFinished(true) {
    std::thread* schedulerThread = new std::thread(&TaskScheduler::SchedulerThread, this);
    this->schedulerThread.reset(schedulerThread);
    this->schedulerThread->detach();
}

void TaskScheduler::RequestSearch(SchedulerTask& task) {
    searchRequests.push_back(task);
    clearTasks = true;
}

bool TaskScheduler::IsSearchFinished() {
    return searchFinished;
}

void TaskScheduler::SchedulerThread() {
    while (true) {
        if (clearTasks) {
            //pool.ClearTasks();
            clearTasks = false;
        }
        else if (!searchRequests.empty()) {
            SchedulerTask task = searchRequests.front();
            searchRequests.pop_front();
            searchFinished = false;
            //MultithreadedSearch(*task.results, task.stringToSearch, task.dictionary, pool);
            Search(*task.results, task.stringToSearch, task.dictionary);
            searchFinished = true;
            std::cout << "Results size: " << task.results->size() << std::endl;
        }
        else {
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
