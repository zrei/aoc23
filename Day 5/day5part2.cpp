#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#define DESTINATION_INDEX 0
#define SOURCE_INDEX 1
#define RANGE_INDEX 2

std::vector<std::string> stringKey = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};
std::map<std::string, std::vector<std::vector<unsigned long>>> mappings = {};
std::vector<std::pair<unsigned long, unsigned long>> seeds = {};
std::vector<std::vector<std::pair<unsigned long, unsigned long>>> layers;
template <typename T>
void printVec(std::vector<T> const& vec);

void parseSeeds(std::string& s);
void parseMappings(std::string const& s, int index);
void mapLocation(unsigned long seedNum, unsigned long* minLocation);
void createLayers();
unsigned long determineSeed();

bool isNumber(char c);
bool comparator (std::vector<unsigned long> i, std::vector<unsigned long> j);

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

    for (std::string key : stringKey) {
        std::sort(mappings[key].begin(), mappings[key].end(), comparator);
    }
    createLayers();
    std::cout << determineSeed();
}

bool isNumber(char c) {
    return c >= 48 and c <= 57;
}

void parseSeeds(std::string& s) {
    std::string numbers = s.substr(s.find(':') + 2);
    int index = 0;
    seeds.push_back({0, 0});
    int pointer = 0;
    bool finishedStartNum = false;
    while (pointer < numbers.size()) {
        if (isNumber(numbers[pointer])) {
            if (finishedStartNum) {
                seeds[index].second *= 10;
                seeds[index].second += numbers[pointer] - '0';
            } else {
                seeds[index].first *= 10;
                seeds[index].first += numbers[pointer] - '0';
            }
        } else if (finishedStartNum) {
            seeds.push_back({0, 0});
            finishedStartNum = false;
            index++;
        } else {
            finishedStartNum = true;
        }
        pointer++;
    }
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
    unsigned long destinationRangeStart = 0;
    for (int i = 0; i < firstSpaceIndex; i++) {
        destinationRangeStart *= 10;
        destinationRangeStart += s[i] - '0';
    }
    unsigned long sourceRangeStart = 0;
    for (int i = firstSpaceIndex + 1; i < lastSpaceIndex; i++) {
        sourceRangeStart *= 10;
        sourceRangeStart += s[i] - '0';
    }
    unsigned long range = 0;
    for (int i = lastSpaceIndex + 1; i < s.size(); i++) {
        range *= 10;
        range += s[i] - '0';
    }
    mappings[stringKey[index]].push_back({destinationRangeStart, sourceRangeStart, range});
}

void mapLocation(unsigned long seedNum, unsigned long* minLocation) {
    unsigned long currNum = seedNum;
    for (int i = 0; i < stringKey.size(); i++) {
        for (std::vector<unsigned long> range : mappings[stringKey[i]]) {
            if (currNum >= range[SOURCE_INDEX] and currNum < range[SOURCE_INDEX] + range[RANGE_INDEX]) {
                currNum = range[DESTINATION_INDEX] + currNum - range[SOURCE_INDEX];
                break;
            }
        }
    }
    (*minLocation) = std::min(*minLocation, currNum);
}

bool comparator(std::vector<unsigned long> i, std::vector<unsigned long> j) {
    return i[DESTINATION_INDEX] < j[DESTINATION_INDEX];
}

void createLayers() {
    std::vector<std::vector<std::pair<unsigned long, unsigned long>>> temp = {};
    int dest = 0;
    for (std::vector<unsigned long> range : mappings[stringKey.back()]) {
        if (dest < range[DESTINATION_INDEX]) {
            temp.push_back({{dest, range[DESTINATION_INDEX] - dest}});
        }
        temp.push_back({{range[SOURCE_INDEX], range[RANGE_INDEX]}});
        dest = range[DESTINATION_INDEX] + range[RANGE_INDEX];
    }
    for (int i = stringKey.size() - 2; i >= 0; i--) {
        std::vector<std::vector<std::pair<unsigned long, unsigned long>>> temp2 = {};
        for (int k = 0; k < temp.size(); k++) {
            temp2.push_back({});
            for (std::pair<unsigned long, unsigned long> j : temp[k]) {
                int index = 0;
                unsigned long dest = j.first;
                unsigned long range = j.second;
                while (true) {
                    if (index >= mappings[stringKey[i]].size()) {
                        temp2[k].push_back({dest, range});
                        break;
                    }
                    std::vector<unsigned long> mappingRange = mappings[stringKey[i]][index];
                    if (dest < mappingRange[DESTINATION_INDEX]) {
                        temp2[k].push_back({dest, std::min(mappingRange[DESTINATION_INDEX] - dest, range)});
                        std::cout << mappingRange[DESTINATION_INDEX] - dest << "\n";
                        if (mappingRange[DESTINATION_INDEX] - dest >= range) {
                            break;
                        }
                        dest = mappingRange[DESTINATION_INDEX];
                    }
                    if (dest >= mappingRange[DESTINATION_INDEX] + mappingRange[RANGE_INDEX]) {
                        index++;
                        continue;
                    }
                    if (dest + range <= mappingRange[DESTINATION_INDEX] + mappingRange[RANGE_INDEX]) {
                        std::pair<unsigned long, unsigned long> tempPair = {mappingRange[SOURCE_INDEX] + (dest - mappingRange[DESTINATION_INDEX]), range};
                        temp2[k].push_back(tempPair);
                        break;
                    } else if (dest + range > mappingRange[DESTINATION_INDEX] + mappingRange[RANGE_INDEX]) {
                        std::pair<unsigned long, unsigned long> tempPair = {mappingRange[SOURCE_INDEX] + dest - mappingRange[DESTINATION_INDEX], mappingRange[RANGE_INDEX] - (dest - mappingRange[DESTINATION_INDEX])};
                        temp2[k].push_back(tempPair);
                        range -= mappingRange[DESTINATION_INDEX] + mappingRange[RANGE_INDEX] - dest;
                        dest = mappingRange[DESTINATION_INDEX] + mappingRange[RANGE_INDEX];
                        index++;
                    }
                }
            }
        }
        temp = temp2;
    }
    layers = temp;
}

unsigned long determineSeed() {
    unsigned long minLocation = UINT_MAX;
    bool found = false;
    for (int i = 0; i < layers.size(); i++) {
        for (std::pair<unsigned long, unsigned long> range : layers[i]) {
            for (std::pair<unsigned long, unsigned long> seedRange : seeds) {
                if (seedRange.first >= range.first and seedRange.first < range.first + range.second) {
                    mapLocation(seedRange.first, &minLocation);
                    found = true;
                } else if (seedRange.first + seedRange.second <= range.first + range.second and seedRange.first + seedRange.second >= range.first) {
                    mapLocation(range.first, &minLocation);
                    found = true;
                }
            }
        }
        if (found)
            break;
    }
    if (not found) {
        for (std::pair<unsigned long, unsigned long> seedRange : seeds) {
            minLocation = std::min(seedRange.first, minLocation);
        }
    }
    return minLocation;
}