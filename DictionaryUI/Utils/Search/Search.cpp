#include "Search.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <unordered_set>

#define DICT_BLOCK_SIZE 8192

Searcher::Searcher() : stopSearch(false) {}

bool Searcher::Contains(std::string str, std::string& stringToSearch) {
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

size_t Searcher::SearchInBlock(int bytesToSearch, char* dictBlock, std::string& stringToSearch, std::vector<std::string>& results) {
    // This loop clears block to avoid getting partly read strings. We will get it in the next block anyway
    size_t offsetForWord = 0;
    for (int i = bytesToSearch; i >= 0; i--) {
        if (dictBlock[i] == '\n') break;
        dictBlock[i] = '\0';
        offsetForWord += 1;
    }


    std::stringstream ss(dictBlock);
    std::string dictString;
    while (std::getline(ss, dictString, '\n')) {
        if (Contains(dictString, stringToSearch)) {
            results.push_back(dictString);
        }
    }

    return offsetForWord;
}

void Searcher::Search(std::vector<std::string>& results, std::string& stringToSearch, FILE* dictionary) {
    if (stringToSearch.empty()) return;

    fseek(dictionary, 0, SEEK_SET);
    const size_t memToAlloc = (size_t)DICT_BLOCK_SIZE * std::thread::hardware_concurrency();
    char* dictBlocks = new char[memToAlloc];
    std::memset(dictBlocks, 0, memToAlloc);
    size_t readRes;
    stopSearch = false;

    // Here we read dictionary file block by block
    while (!stopSearch) {
        for (size_t i = 0; i < memToAlloc; i += DICT_BLOCK_SIZE) {
            readRes = fread(dictBlocks + i, sizeof(char), DICT_BLOCK_SIZE, dictionary);
            if (readRes != DICT_BLOCK_SIZE) {
                fseek(dictionary, -SearchInBlock(readRes, dictBlocks + i, stringToSearch, results), SEEK_CUR);
                stopSearch = true;
            }
            else {
                fseek(dictionary, -SearchInBlock(DICT_BLOCK_SIZE - 1, dictBlocks + i, stringToSearch, results), SEEK_CUR);
            }
            if (stopSearch) break;
        }

    }

    delete[] dictBlocks;
}

void Searcher::StopSearch() {
    stopSearch = true;
}
