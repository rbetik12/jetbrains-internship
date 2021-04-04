#include "TaskScheduler.h"
#include <chrono>
#include <thread>
#include <iostream>
#include "../Search/Search.h"

TaskScheduler::TaskScheduler(): clear(false), searchFinished(true) {
    std::thread* schedulerThread = new std::thread(&TaskScheduler::SchedulerThread, this);
    this->schedulerThread.reset(schedulerThread);
    this->schedulerThread->detach();
}

void TaskScheduler::RequestSearch(SchedulerTask& task) {
    searchRequests.push_back(task);
}

bool TaskScheduler::IsSearchFinished() {
    return searchFinished;
}

void TaskScheduler::Clear() {
    clear = true;
}

void TaskScheduler::SchedulerThread() {
    while (true) {
        if (clear) {
            clear = false;
            searcher.StopSearch();
            searchFinished = true;
        }
        if (!searchRequests.empty()) {
            SchedulerTask task = searchRequests.front();
            searchRequests.pop_front();
            searchFinished = false;
            searcher.Search(*task.results, task.stringToSearch, task.dictionary);
            if (clear) {
                task.results->clear();
                clear = true;
            }
            searchFinished = true;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
