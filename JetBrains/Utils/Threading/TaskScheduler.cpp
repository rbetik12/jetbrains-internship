#include "TaskScheduler.h"
#include <chrono>
#include <thread>
#include <iostream>

TaskScheduler::TaskScheduler() {
    std::thread* schedulerThread = new std::thread(&TaskScheduler::SchedulerThread, this);
    this->schedulerThread.reset(schedulerThread);
    this->schedulerThread->detach();
}

void TaskScheduler::RequestSearch(SchedulerTask& task) {
    searchRequests.push_back(task);
}

void TaskScheduler::SchedulerThread() {
    while (true) {
        if (!searchRequests.empty()) {
            SchedulerTask task = searchRequests.front();
            searchRequests.pop_front();
            std::cout << "Schdeuler thread received new search task! String to find: " << task.stringToSearch << std::endl;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
