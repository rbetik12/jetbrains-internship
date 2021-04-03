#include "Search.h"
#include <iostream>
#include <sstream>
#include "../Threading/ThreadTask.h"

#define DICT_BLOCK_SIZE 256

bool Contains(std::string str, std::string& stringToSearch) {
    int t = 0;
    int len = str.length();

    for (int i = 0; i < len; i++) {
        if (t == stringToSearch.length())
            break;
        if (str[i] == stringToSearch[t])
            t++;
        else
            t = 0;
    }

    return t < stringToSearch.length() ? false : true;
}

void SearchInBlock(int bytesToSearch, char* dictBlock, std::string& stringToSearch, std::vector<std::string>& results) {
    for (int i = bytesToSearch; i >= 0; i--) {
        if (dictBlock[i] == '\n') break;
        else dictBlock[i] = '\0';
    }

    std::stringstream ss(dictBlock);
    std::string dictString;
    while (std::getline(ss, dictString, '\n')) {
        if (Contains(dictString, stringToSearch)) {
            results.push_back(dictString);
        }
    }
}

void Search(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary) {
    if (stringToSearch.empty()) return;

    fseek(dictionary, 0, SEEK_SET);
    char* dictBlock = new char[DICT_BLOCK_SIZE];
    long long readRes;

    while ((readRes = fread(dictBlock, sizeof(char), DICT_BLOCK_SIZE, dictionary)) == DICT_BLOCK_SIZE) {
        SearchInBlock(DICT_BLOCK_SIZE - 1, dictBlock, stringToSearch, results);
    }
    SearchInBlock(readRes, dictBlock, stringToSearch, results);

    delete[] dictBlock;
}

void MultithreadedSearch(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary, ThreadPool& pool) {
    if (stringToSearch.empty()) return;

    fseek(dictionary, 0, SEEK_SET);
    char* dictBlock = new char[DICT_BLOCK_SIZE];
    long long readRes;
    ThreadTask task;

    while ((readRes = fread(dictBlock, sizeof(char), DICT_BLOCK_SIZE, dictionary)) == DICT_BLOCK_SIZE) {
        task.block = dictBlock;
        task.bytesToSearch = DICT_BLOCK_SIZE - 1;
        task.results = &results;
        task.stringToSearch = &stringToSearch;
        pool.AddTask(task);
        dictBlock = new char[DICT_BLOCK_SIZE];
    }
    SearchInBlock(readRes, dictBlock, stringToSearch, results);
    task.block = dictBlock;
    task.bytesToSearch = readRes;
    task.results = &results;
    task.stringToSearch = &stringToSearch;
    pool.AddTask(task);
}
