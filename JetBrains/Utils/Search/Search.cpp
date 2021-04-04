#include "Search.h"
#include <iostream>
#include <sstream>
#include "../Threading/ThreadTask.h"
#include <chrono>
#include <thread>
#include <unordered_set>

#define DICT_BLOCK_SIZE 8192

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
    /*char* dictBlock = new char[DICT_BLOCK_SIZE];
    long long readRes;

    while ((readRes = fread(dictBlock, sizeof(char), DICT_BLOCK_SIZE, dictionary)) == DICT_BLOCK_SIZE) {
        SearchInBlock(DICT_BLOCK_SIZE - 1, dictBlock, stringToSearch, results);
    }
    SearchInBlock(readRes, dictBlock, stringToSearch, results);

    delete[] dictBlock;*/

    const size_t memToAlloc = (size_t)DICT_BLOCK_SIZE * std::thread::hardware_concurrency();
    char* dictBlocks = new char[memToAlloc];
    bool stopSearch = false;
    size_t readRes;
    while (!stopSearch) {
        for (size_t i = 0; i < memToAlloc; i += DICT_BLOCK_SIZE) {
            readRes = fread(dictBlocks + i, sizeof(char), DICT_BLOCK_SIZE, dictionary);
            if (readRes != DICT_BLOCK_SIZE) {
                SearchInBlock(readRes, dictBlocks + i, stringToSearch, results);
                stopSearch = true;
            }
            else {
                SearchInBlock(DICT_BLOCK_SIZE - 1, dictBlocks + i, stringToSearch, results);
            }
            if (stopSearch) break;
        }

    }

    delete[] dictBlocks;
}

void MultithreadedSearch(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary, ThreadPool& pool) {
    if (stringToSearch.empty()) return;

    fseek(dictionary, 0, SEEK_SET);
    //char* dictBlock = new char[DICT_BLOCK_SIZE];
    long long readRes;
    ThreadTask task;

    /*while ((readRes = fread(dictBlock, sizeof(char), DICT_BLOCK_SIZE, dictionary)) == DICT_BLOCK_SIZE) {
        task.block = dictBlock;
        task.bytesToSearch = DICT_BLOCK_SIZE - 1;
        task.results = &results;
        task.stringToSearch = &stringToSearch;
        pool.AddTask(task);
        dictBlock = new char[DICT_BLOCK_SIZE];
    }
    task.block = dictBlock;
    task.bytesToSearch = readRes;
    task.results = &results;
    task.stringToSearch = &stringToSearch;
    pool.AddTask(task);*/

    const size_t memToAlloc = (size_t)DICT_BLOCK_SIZE * std::thread::hardware_concurrency();
    char* dictBlocks = new char[memToAlloc];
    bool stopSearch = false;
    task.stringToSearch = &stringToSearch;
    task.results = &results;
    while (!stopSearch) {

        if (!pool.IsFinished()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            continue;
        }

        for (size_t i = 0; i < memToAlloc; i += DICT_BLOCK_SIZE) {
            std::cout << ftell(dictionary) << std::endl;
            readRes = fread(dictBlocks + i, sizeof(char), DICT_BLOCK_SIZE, dictionary);
            if (readRes != DICT_BLOCK_SIZE) {
                task.bytesToSearch = readRes;
                stopSearch = true;
            }
            else {
                task.bytesToSearch = DICT_BLOCK_SIZE - 1;
            }
            task.block = dictBlocks + i;
            task.blockID = ftell(dictionary);
            pool.AddTask(task);

            if (stopSearch) break;
        }

    }

    while(!pool.IsFinished()) {}

    delete[] dictBlocks;
}
