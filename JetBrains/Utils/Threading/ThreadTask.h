#pragma once
#include <string>
#include <vector>

struct ThreadTask {
    int bytesToSearch; 
    std::string* stringToSearch; 
    std::vector<std::string>* results;
    char* block;
};

struct SchedulerTask {
    std::vector<std::string>* results;
    std::string stringToSearch;
    FILE* dictionary;
};