#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#define SPRING '#'
#define BROKEN_SPRING '.'
#define UNKNOWN '?'

int processLine(std::string s);
std::vector<int> processSpringNumbers(std::string const&s);
std::vector<std::vector<bool>> processSprings(std::string const& s);
bool checkPerm(std::vector<bool> const& perm, std::vector<int> numbers, std::string const& springs);
std::ofstream writer;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No file name provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);
    int finalOutput = 0;
    writer.open("output1.txt");
    if (myFile.is_open()) {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            finalOutput += processLine(s);
        }
    }
    writer.close();
    std::cout << finalOutput << "\n";
}

int processLine(std::string s) {
    int spaceIndex = s.find(' ');
    std::vector<std::vector<bool>>  perms = processSprings(s.substr(0, spaceIndex));
    std::vector<int> numbers = processSpringNumbers(s.substr(spaceIndex + 1));
    int numCombinations = 0;
    for (std::vector<bool> perm : perms) {
        if (checkPerm(perm, numbers, s.substr(0, spaceIndex)))
            numCombinations++;
    }
    //writer << numCombinations << "\n";
    std::cout << s << ", " << numCombinations << "\n";
    return numCombinations;
}

std::vector<int> processSpringNumbers(std::string const&s) {
    int lp = 0;
    int rp = 0;
    int currNum = 0;
    std::vector<int> numbers = {};
    while (rp < s.size()) {
        if (s[rp] == ',') {
            numbers.push_back(currNum);
            currNum = 0;
            lp = rp + 1;
        } else {
            currNum *= 10;
            currNum += s[rp] - '0';
        }
        rp++;
    }
    numbers.push_back(currNum);
    return numbers;
}

std::vector<std::vector<bool>> processSprings(std::string const& s) {
    int numUnknowns = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == UNKNOWN)
            numUnknowns++;
    }
    std::vector<std::vector<bool>> perms = {};
    for (int i = 0; i < numUnknowns; i++) {
        if (i == 0) {
            perms.push_back({true});
            perms.push_back({false});
        } else {
            int tempSize = perms.size();
            for (int j = 0; j < tempSize; j++) {
                std::vector<bool> temp = perms[j];
                perms[j].push_back(true);
                temp.push_back(false);
                perms.push_back(temp);
            }
        }
    }
    return perms;
}

bool checkPerm(std::vector<bool> const& perm, std::vector<int> numbers, std::string const& springs) {
    int lp = 0;
    int rp = 0;
    int unknownCounter = 0;
    int brokenSpringCounter = 0;
    while (rp < springs.size()) {
        if (springs[rp] == BROKEN_SPRING) {
            if (rp > lp) {
                if (brokenSpringCounter >= numbers.size() or !(numbers[brokenSpringCounter] == (rp - lp)))
                    return false;
                brokenSpringCounter++;
            }
            lp = rp + 1;
        } else if ((springs[rp] == UNKNOWN and !perm[unknownCounter])) {
            if (rp > lp) {
                if (brokenSpringCounter >= numbers.size() or !(numbers[brokenSpringCounter] == (rp - lp)))
                    return false;
                brokenSpringCounter++;
            }
            unknownCounter++;
            lp = rp + 1;
        } else if (springs[rp] == UNKNOWN) {
            unknownCounter++;
        }
        rp++;
    }
    if (rp > lp) {
        if (brokenSpringCounter >= numbers.size() or !(numbers[brokenSpringCounter] == (rp - lp)))
            return false;
        brokenSpringCounter++;
    }
    
    if (brokenSpringCounter < numbers.size()) {
        return false;
    }

    return true;
}