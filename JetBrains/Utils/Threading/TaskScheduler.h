#pragma once
#include "ThreadPool.h"
#include <deque>

class TaskScheduler {
public:
    TaskScheduler();
    void RequestSearch(SchedulerTask& task);
private:
    //ThreadPool pool;
    std::deque<SchedulerTask> searchRequests;
    std::unique_ptr<std::thread> schedulerThread;
    void SchedulerThread();
};