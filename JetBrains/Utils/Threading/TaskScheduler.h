#pragma once
#include "ThreadPool.h"
#include <deque>

class TaskScheduler {
public:
    TaskScheduler();
    void RequestSearch(SchedulerTask& task);
    bool IsSearchFinished();
private:
    //ThreadPool pool;
    std::deque<SchedulerTask> searchRequests;
    bool clearTasks;
    bool searchFinished;
    std::unique_ptr<std::thread> schedulerThread;
    void SchedulerThread();
};