#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#define DESTINATION_INDEX 0
#define SOURCE_INDEX 1
#define RANGE_INDEX 2

std::vector<std::string> stringKey = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};
std::map<std::string, std::vector<std::vector<unsigned int>>> mappings = {};
std::vector<unsigned int> seeds = {};

template <typename T>
void printVec(std::vector<T> const& vec);

void parseSeeds(std::string& s);
void parseMappings(std::string const& s, int index);
void mapLocation(unsigned int seedNum, unsigned int* minLocation);

bool isNumber(char c);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }
    int stringKeyIndex = -1;
    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, s);
            if (myFile.eof()) {
                break;
            }
            if (stringKeyIndex < 0) {
                parseSeeds(s);
                std::getline(myFile, s);
                std::getline(myFile, s);
                stringKeyIndex++;
            }
            else {
                if (s.size() <= 1) {
                    stringKeyIndex++;
                    std::getline(myFile, s);
                    continue;
                }
                parseMappings(s, stringKeyIndex);
            }
        }
    }

    unsigned int minLocationNumber = UINT_MAX;
    for (unsigned int seed : seeds) {
        mapLocation(seed, &minLocationNumber);
    }
    std::cout << minLocationNumber << "\n";
}

bool isNumber(char c) {
    return c >= 48 and c <= 57;
}

void parseSeeds(std::string& s) {
    std::string numbers = s.substr(s.find(':') + 2);
    unsigned int currNum = 0;
    int pointer = 0;
    while (pointer < numbers.size()) {
        if (isNumber(numbers[pointer])) {
            currNum *= 10;
            currNum += numbers[pointer] - '0';
        } else {
            seeds.push_back(currNum);
            currNum = 0;
        }
        pointer++;
    }
    seeds.push_back(currNum);
}

template <typename T>
void printVec(std::vector<T> const& vec) {
    std::cout << "Printing vector: ";
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

void parseMappings(std::string const& s, int index) {
    if (!mappings.count(stringKey[index]))
        mappings[stringKey[index]] = {};
    int firstSpaceIndex = s.find(' ');
    int lastSpaceIndex = s.rfind(' ');
    unsigned int destinationRangeStart = 0;
    for (int i = 0; i < firstSpaceIndex; i++) {
        destinationRangeStart *= 10;
        destinationRangeStart += s[i] - '0';
    }
    unsigned int sourceRangeStart = 0;
    for (int i = firstSpaceIndex + 1; i < lastSpaceIndex; i++) {
        sourceRangeStart *= 10;
        sourceRangeStart += s[i] - '0';
    }
    unsigned int range = 0;
    for (int i = lastSpaceIndex + 1; i < s.size(); i++) {
        range *= 10;
        range += s[i] - '0';
    }
    mappings[stringKey[index]].push_back({destinationRangeStart, sourceRangeStart, range});
}

void mapLocation(unsigned int seedNum, unsigned int* minLocation) {
    unsigned int currNum = seedNum;
    for (int i = 0; i < stringKey.size(); i++) {
        for (std::vector<unsigned int> range : mappings[stringKey[i]]) {
            if (currNum >= range[SOURCE_INDEX] and currNum < range[SOURCE_INDEX] + range[RANGE_INDEX]) {
                currNum = range[DESTINATION_INDEX] + currNum - range[SOURCE_INDEX];
                break;
            }
        }
    }
    (*minLocation) = std::min(*minLocation, currNum);
}