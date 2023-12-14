#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#define SPRING '#'
#define BROKEN_SPRING '.'
#define UNKNOWN '?'

long long processLine(std::string s);
std::vector<int> processSpringNumbers(std::string const&s);
long long helper(std::string const& s, int stringIndex, int combinationIndex, std::vector<int> const& combinations, std::map<std::pair<int, int>, long long>& visited);
std::ofstream writer;
std::string concatenateString(std::string const& s);

int main(int argc, char* argv[]) {
    std::cout << "Attempt 10\n";
    if (argc < 2) {
        std::cout << "No file name provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);
    long long finalOutput = 0;
    if (myFile.is_open()) {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            finalOutput += processLine(s);
        }
    }
    std::cout << finalOutput << "\n";
}

std::string concatenateString(std::string const& s) {
    std::string finalResult = "";
    for (int i = 0; i < 4; i++) {
        finalResult += s;
        finalResult += "?";
    }
    finalResult += s;
    return finalResult;
}

long long processLine(std::string s) {
    int spaceIndex = s.find(' ');
    std::map<std::pair<int, int>, long long> memo = {};
    std::vector<int> numbers = processSpringNumbers(s.substr(spaceIndex + 1));
    std::string finalString = concatenateString(s.substr(0, spaceIndex));
    long long output = helper(finalString, 0, 0, numbers, memo);
    std::cout << finalString << ", " << output << "\n";
    return output;
}

std::vector<int> processSpringNumbers(std::string const&s) {
    int lp = 0;
    int rp = 0;
    int currNum = 0;
    std::vector<int> temp = {};
    while (rp < s.size()) {
        if (s[rp] == ',') {
            temp.push_back(currNum);
            currNum = 0;
            lp = rp + 1;
        } else {
            currNum *= 10;
            currNum += s[rp] - '0';
        }
        rp++;
    }
    temp.push_back(currNum);
    std::vector<int> numbers = {};
    for (int i = 0; i < 5; i++) {
        for (int c : temp)
            numbers.push_back(c);
    }
    return numbers;
}

long long helper(std::string const& s, int stringIndex, int combinationIndex, std::vector<int> const& combinations, std::map<std::pair<int, int>, long long>& memo) {
    if (memo.count({stringIndex, combinationIndex}))
        return memo[{stringIndex, combinationIndex}];
    long long additionalCombinations = 0;
    int pointer = stringIndex;
    int combiPointer = combinationIndex;
    while (pointer < s.size()) {
        if (s[pointer] == SPRING) {
            if (combiPointer >= combinations.size()) {
                memo[{stringIndex, combinationIndex}] = additionalCombinations;
                return additionalCombinations;
            }
            for (int i = 1; i < combinations[combiPointer]; i++) {
                if (pointer + i >= s.size() or s[pointer + i] == BROKEN_SPRING) {
                    memo[{stringIndex, combinationIndex}] = additionalCombinations;
                    return additionalCombinations;
                }
            }
            if (pointer + combinations[combiPointer] - 1 < s.size() - 1 and s[pointer + combinations[combiPointer]] == SPRING) {
                memo[{stringIndex, combinationIndex}] = additionalCombinations;
                return additionalCombinations;
            }
            pointer += combinations[combiPointer] + 1;
            combiPointer++;
        } else if (s[pointer] == UNKNOWN) {
            if (combiPointer >= combinations.size()) {
                pointer++;
                continue;
            }
            bool shouldSkip = false;
            for (int i = 1; i < combinations[combiPointer]; i++) {
                if (pointer + i >= s.size() or s[pointer + i] == BROKEN_SPRING) {
                    pointer++;
                    shouldSkip = true;
                    break;
                }
            }
            if (shouldSkip)
                continue;
            if (pointer + combinations[combiPointer] - 1 < s.size() - 1 and s[pointer + combinations[combiPointer]] == SPRING) {
                pointer++;
                continue;
            }
            additionalCombinations += helper(s, pointer + 1, combiPointer, combinations, memo);
            pointer += combinations[combiPointer] + 1;
            combiPointer++;
        } else {
            pointer++;
        }
    }
    if (combiPointer >= combinations.size()) {
        memo[{stringIndex, combinationIndex}] = additionalCombinations + 1;
        return additionalCombinations + 1;
    }
    else {
        memo[{stringIndex, combinationIndex}] = additionalCombinations;
        return additionalCombinations;
    }
}