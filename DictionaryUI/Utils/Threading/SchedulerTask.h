#pragma once
#include <string>
#include <unordered_set>
#include <vector>

struct SchedulerTask {
    std::vector<std::string>* results;
    std::string stringToSearch;
    FILE* dictionary;
};