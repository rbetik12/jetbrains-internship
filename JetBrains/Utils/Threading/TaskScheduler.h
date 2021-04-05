#pragma once
#include <deque>
#include <thread>
#include "../Search/Search.h"
#include "SchedulerTask.h"

// This class schedules search operation to do it in a separate thread
class TaskScheduler {
public:
    TaskScheduler();
    void RequestSearch(SchedulerTask& task);
    bool IsSearchFinished();
    void Clear();
private:
    Searcher searcher;
    std::deque<SchedulerTask> searchRequests;
    bool clear;
    bool searchFinished;
    std::unique_ptr<std::thread> schedulerThread;
    void SchedulerThread();
};