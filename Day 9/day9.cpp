#include <iostream>
#include <vector>
#include <fstream>
#include <string>

template <typename T>
void printVec(std::vector<T> vec);

void parseLine(std::vector<int>& sequence, std::string s);

int processLine(std::string s);

bool allZero(std::vector<int> const& sequence);

std::vector<int> findNextSequence(std::vector<int> const& sequence);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    int finalOutput = 0;

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

template <typename T>
void printVec(std::vector<T> vec) {
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

void parseLine(std::vector<int>& sequence, std::string s) {
    int spaceIndex = s.find(' ');
    while (spaceIndex != -1) {
        sequence.push_back(std::stoi(s.substr(0, spaceIndex)));
        s = s.substr(spaceIndex + 1);
        spaceIndex = s.find(' ');
    }
    sequence.push_back(std::stoi(s));
}

int processLine(std::string s) {
    std::vector<int> lastNumber = {};
    std::vector<int> sequence = {};
    parseLine(sequence, s);
    while (!allZero(sequence)) {
        lastNumber.push_back(sequence.back());
        sequence = findNextSequence(sequence);
    }
    int final = 0;
    for (int i = lastNumber.size() - 1; i >= 0; i--) {
        final += lastNumber[i];
    }
    return final;
}

bool allZero(std::vector<int> const& sequence) {
    for (int i : sequence) {
        if (i != 0)
            return false;
    }
    return true;
}

std::vector<int> findNextSequence(std::vector<int> const& sequence) {
    std::vector<int> newSequence = {};
    for (int i = 1; i < sequence.size(); i++) {
        newSequence.push_back(sequence[i] - sequence[i - 1]);
    }
    return newSequence;
}