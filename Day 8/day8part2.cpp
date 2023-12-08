#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <numeric>

#define STARTING_CHAR 'A'
#define ENDING_CHAR 'Z'

std::string instructions;
std::map<std::string, std::pair<std::string, std::string>> graph = {};
std::vector<std::string> allPoints = {};
std::vector<unsigned long> allPointsDistance = {};

void processLine(std::string s1, std::string s2, std::string s3);
std::string getLeft(std::pair<std::string, std::string> p);
std::string getRight(std::pair<std::string, std::string> p);
bool isLeft(int index);
unsigned long findNumberOfSteps();
void findDistances();

template <typename T>
void printVec(std::vector<T> vec);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }
    std::ifstream myFile;
    myFile.open(argv[1]);
    if (!myFile.is_open())
        return 0;

    myFile >> instructions;

    std::string s1;
    std::string s2;
    std::string s3;

    while (myFile.good()) {
        myFile >> s1;
        if (myFile.eof())
            break;
        myFile >> s2;
        myFile >> s2;
        myFile >> s3;
        processLine(s1, s2, s3);
    }
    findDistances();
    std::cout << findNumberOfSteps() << "\n";
}

void processLine(std::string s1, std::string s2, std::string s3) {
    graph[s1] = {s2.substr(1, s2.size() - 2), s3.substr(0, s3.size() - 1)};
    if (s1[s1.size() - 1] == STARTING_CHAR)
        allPoints.push_back(s1);
}

std::string getLeft(std::pair<std::string, std::string> p) {
    return p.first;
}

std::string getRight(std::pair<std::string, std::string> p) {
    return p.second;
}

bool isLeft(int index) {
    return instructions[index] == 'L';
}

unsigned long findNumberOfSteps() {
    unsigned long result = 1;
    for (unsigned long i : allPointsDistance) {
        result = std::lcm(i, result);
    }
    return result;
}

template <typename T>
void printVec(std::vector<T> vec) {
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

void findDistances() {
    for (std::string s : allPoints)
    {
        std::string currPoint = s;
        unsigned long numPointsTraversed = 0;
        int index = 0;
        while (currPoint[currPoint.size() - 1] != 'Z') {
            if (isLeft(index))
                currPoint = getLeft(graph[currPoint]);
            else
                currPoint = getRight(graph[currPoint]);
            index = (index + 1) % instructions.size();
            numPointsTraversed++;
        }
        allPointsDistance.push_back(numPointsTraversed);
    }
}
