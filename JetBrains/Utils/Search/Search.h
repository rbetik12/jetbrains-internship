#pragma once
#include <vector>
#include <string>
#include "../Threading/ThreadPool.h"

void Search(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary);

void MultithreadedSearch(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary, ThreadPool& pool);

bool Contains(std::string str, std::string& stringToSearch);