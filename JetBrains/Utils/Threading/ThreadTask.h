#pragma once
#include <string>
#include <unordered_set>
#include <vector>

struct ThreadTask {
    int bytesToSearch; 
    std::string* stringToSearch; 
    std::vector<std::string>* results;
    char* block;
    uint64_t blockID;
};

struct SchedulerTask {
    std::vector<std::string>* results;
    std::string stringToSearch;
    FILE* dictionary;
};