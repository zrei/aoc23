#include <iostream>
#include <map>
#include <fstream>
#include <string>

#define STARTING_POINT "AAA"
#define ENDING_POINT "ZZZ"

std::string instructions;
std::map<std::string, std::pair<std::string, std::string>> graph = {};

void processLine(std::string s1, std::string s2, std::string s3);
std::string getLeft(std::pair<std::string, std::string> p);
std::string getRight(std::pair<std::string, std::string> p);
bool isLeft(int index);
int traverseGraph();

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

    std::cout << traverseGraph() << "\n";
}

void processLine(std::string s1, std::string s2, std::string s3) {
    graph[s1] = {s2.substr(1, s2.size() - 2), s3.substr(0, s3.size() - 1)};
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

int traverseGraph() {
    int numPointsTraversed = 0;
    std::string currPoint = STARTING_POINT;
    int index = 0;
    while (currPoint != ENDING_POINT) {
        if (isLeft(index))
            currPoint = getLeft(graph[currPoint]);
        else
            currPoint = getRight(graph[currPoint]);
        index = (index + 1) % instructions.size();
        numPointsTraversed++;
    }
    return numPointsTraversed;
}