#pragma once
#include <vector>
#include <string>
#include <unordered_set>

// This class is responsible for searching
class Searcher {
public:
    Searcher();
    void Search(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary);
    void StopSearch();
private:
    bool Contains(std::string str, std::string& stringToSearch);
    size_t SearchInBlock(int bytesToSearch, char* dictBlock, std::string& stringToSearch, std::vector<std::string>& results);
    bool stopSearch;
};