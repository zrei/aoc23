#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>
#include <map>

// the smart way to do this is with shoestring and pick's theorem but i'm just doing it with a priority queue :D

enum direction {
    LEFT = '2',
    RIGHT = '0',
    UP = '3',
    DOWN = '1',
    NONE = 0
};

struct instruction {
    direction dir;
    long long movementAmount;

    instruction(direction d, long long m) {
        dir = d;
        movementAmount = m;
    }
};

std::vector<instruction> instructions = {};
std::pair<int, int> startingPoint = {0, 0};

struct customComparator {
    bool operator()(const std::pair<int, direction> l, const std::pair<int, direction> r) const { return -l.first < -r.first; }
};

std::map<int, std::priority_queue<std::pair<int, direction>, std::vector<std::pair<int, direction>>, customComparator>> mapper = {};
std::map<int, std::map<std::pair<int, int>, int>> restrictedRanges = {};
int minRow = 0;
int maxRow = 0;

void processLine(std::string s);
void dig(long long* trenchSize);
void followInstruction(instruction instr, std::pair<int, int>& coordinates, long long* trenchSize);
void countTrench(long long* trenchSize);
long long decodeHexadecimalInstruction(std::string hexadecimalInstruction);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }
    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            processLine(s);
        }
    }
    long long finalOutput = 0;
    dig(&finalOutput);
    countTrench(&finalOutput);
    std::cout << finalOutput << "\n";
}

void processLine(std::string s) {
    int spaceIndex = s.rfind(' ');
    std::string hexadecimalInstruction = s.substr(spaceIndex + 3);
    std::string hexadecimalDistance = hexadecimalInstruction.substr(0, hexadecimalInstruction.size() - 2);
    char hexadecimalDirection = hexadecimalInstruction[hexadecimalInstruction.size() - 2];
    long long movementAmount = decodeHexadecimalInstruction(hexadecimalDistance);
    std::cout << hexadecimalDirection << ", " << movementAmount << "\n";
    switch (hexadecimalDirection) {
        case LEFT:
            instructions.push_back(instruction(LEFT, movementAmount));
            break;
        case RIGHT:
            instructions.push_back(instruction(RIGHT, movementAmount));
            break;
        case UP:
            instructions.push_back(instruction(UP, movementAmount));
            break;
        case DOWN:
            instructions.push_back(instruction(DOWN, movementAmount));
            break;
        default:
            break;
    }
}

long long decodeHexadecimalInstruction(std::string hexadecimalInstruction) {
    long long finalOutput = 0;
    int power = 0;
    int base = 16;
    std::string index = "0123456789abcdef";
    for (int i = hexadecimalInstruction.size() - 1; i >= 0; i--) {
        finalOutput += index.find(hexadecimalInstruction[i]) * (std::pow(base, power));
        power++;
    }
    return finalOutput;
}

void followInstruction(instruction instr, std::pair<int, int>& coordinates, long long* trenchSize) {
    (*trenchSize) += instr.movementAmount;
    int initialCol;
    int finalCol;
    switch (instr.dir) {
        case UP:
            for (int row = 1; row <= instr.movementAmount; row++) {
                mapper[coordinates.first].push({coordinates.second, instr.dir});
                coordinates.first -= 1;
            }
            mapper[coordinates.first].push({coordinates.second, instr.dir});
            minRow = std::min(minRow, coordinates.first);
            break;
        case DOWN:
            for (int row = 1; row <= instr.movementAmount; row++) {
                mapper[coordinates.first].push({coordinates.second, instr.dir});
                coordinates.first += 1;
            }
            mapper[coordinates.first].push({coordinates.second, instr.dir});
            maxRow = std::max(maxRow, coordinates.first);
            break;
        case LEFT:
            initialCol = coordinates.second;
            for (int col = 1; col <= instr.movementAmount; col++) {
                coordinates.second -= 1;
            }
            finalCol = coordinates.second;
            if (!restrictedRanges.count(coordinates.first))
                restrictedRanges[coordinates.first] = {};
            restrictedRanges[coordinates.first][{finalCol, initialCol}] = 1;
            break;
        case RIGHT:
            initialCol = coordinates.second;
            for (int col = 1; col <= instr.movementAmount; col++) {
                coordinates.second += 1;
            }
            finalCol = coordinates.second;
            if (!restrictedRanges.count(coordinates.first))
                restrictedRanges[coordinates.first] = {};
            restrictedRanges[coordinates.first][{initialCol, finalCol}] = 1;
            break;
        default:
            break;
    }
}

void dig(long long* finalOutput) {
    for (instruction i : instructions) {
        std::cout << "Doing instruction\n";
        followInstruction(i, startingPoint, finalOutput);
    }
    std::cout << "Done digging?\n";
    std::cout << "Min row, max row: " << minRow << ", " << maxRow << "\n";
}

void countTrench(long long* trenchSize) {
    for (int row = minRow; row <= maxRow; row++) {
        std::cout << "\nDoing row: " << row << "\n";
        direction leftDir = NONE;
        long long prevWallCol = 0;
        bool withinTrench = false;
        std::priority_queue q = mapper[row];
        while (!q.empty()) {
            std::pair<int, direction> p = q.top();
            q.pop();
            if (leftDir == NONE) {
                leftDir = p.second;
                prevWallCol = p.first;
                withinTrench = true;
            } else if (p.second == leftDir) {
                prevWallCol = p.first;
                withinTrench = true;
            } else {
                if (!withinTrench)
                    continue;
                withinTrench = false;
                if (restrictedRanges.count(row))
                    if (restrictedRanges[row].count({prevWallCol, p.first}))
                        continue;
                (*trenchSize) += p.first - prevWallCol - 1;
            }
        }
    }
}