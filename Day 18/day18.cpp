#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#define EMPTY_SPACE '.'
#define TRENCH '#'

enum direction {
    LEFT = 'L',
    RIGHT = 'R',
    UP = 'U',
    DOWN = 'D',
    NONE = 0
};

struct instruction {
    direction dir;
    int movementAmount;

    instruction(direction d, int m) {
        dir = d;
        movementAmount = m;
    }
};

std::vector<std::vector<char>> space = {};
std::vector<std::vector<std::vector<direction>>> directions = {};
std::vector<instruction> instructions = {};
std::pair<int, int> startingPoint = {0, 0};

int downMovement;
int upMovement;
int leftMovement;
int rightMovement;

int numRows;
int numCols;

void processLine(std::string s);
void createSpace();
void dig();
void followInstruction(instruction instr, std::pair<int, int>& coordinates);
long long countTrench();
bool oppositeDirection(direction leftDir, std::vector<direction> dirs);
bool sameDirection(direction leftDir, std::vector<direction> dirs);
void setLeftDir(direction* leftDir, std::vector<direction> dirs);

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
    createSpace();
    dig();
    std::cout << countTrench() << "\n";
}

void processLine(std::string s) {
    char dir = s[0];
    int movementAmount = std::stoi(s.substr(2, s.substr(2).find(' ')));
    std::cout << dir << ", " << movementAmount << "\n";
    switch (dir) {
        case LEFT:
            instructions.push_back(instruction(LEFT, movementAmount));
            leftMovement += movementAmount;
            break;
        case RIGHT:
            instructions.push_back(instruction(RIGHT, movementAmount));
            rightMovement += movementAmount;
            break;
        case UP:
            instructions.push_back(instruction(UP, movementAmount));
            upMovement += movementAmount;
            break;
        case DOWN:
            instructions.push_back(instruction(DOWN, movementAmount));
            downMovement += movementAmount;
            break;
        default:
            break;
    }
}

void createSpace() {
    numRows = std::max(1 + upMovement, 1 + downMovement) * 2;
    numCols = std::max(1 + leftMovement, 1 + rightMovement) * 2;
    std::cout << "Creating graph\n";
    std::cout << numRows << ", " << numCols << "\n";
    for (int row = 0; row < numRows; row++) {
        space.push_back({});
        directions.push_back({});
        for (int col = 0; col < numCols; col++) {
            space[row].push_back(EMPTY_SPACE);
            directions[row].push_back({});
        }
    }
}

void dig() {
    std::pair<int, int> coordinates = {numRows / 2, numCols / 2};
    startingPoint = {numRows / 2, numCols / 2};
    std::cout << "Starting point: " << coordinates.first << ", " << coordinates.second << "\n";
    space[coordinates.first][coordinates.second] = TRENCH;
    for (instruction instr : instructions) {
        followInstruction(instr, coordinates);
    }
    std::ofstream writer;
    writer.open("test.out");
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            writer << space[row][col];
        }
        writer << "\n";
    }
}

void followInstruction(instruction instr, std::pair<int, int>& coordinates) {
    switch (instr.dir) {
        case UP:
            for (int row = 1; row <= instr.movementAmount; row++) {
                directions[coordinates.first][coordinates.second].push_back(instr.dir);
                coordinates.first -= 1;
                space[coordinates.first][coordinates.second] = TRENCH;
            }
            break;
        case DOWN:
            for (int row = 1; row <= instr.movementAmount; row++) {
                directions[coordinates.first][coordinates.second].push_back(instr.dir);
                coordinates.first += 1;
                space[coordinates.first][coordinates.second] = TRENCH;
            }
            break;
        case LEFT:
            for (int col = 1; col <= instr.movementAmount; col++) {
                directions[coordinates.first][coordinates.second].push_back(instr.dir);
                coordinates.second -= 1;
                space[coordinates.first][coordinates.second] = TRENCH;
            }
            break;
        case RIGHT:
            for (int col = 1; col <= instr.movementAmount; col++) {
                directions[coordinates.first][coordinates.second].push_back(instr.dir);
                coordinates.second += 1;
                space[coordinates.first][coordinates.second] = TRENCH;
            }
            break;
        default:
            break;
    }
    directions[coordinates.first][coordinates.second].push_back(instr.dir);
}

long long countTrench() {
    long long trenchSize = 0;
    for (int row = 0; row < numRows; row++) {
        direction leftDir = NONE;
        bool withinTrench = false;
        for (int col = 0; col < numCols; col++) {
            if (space[row][col] == TRENCH and leftDir != NONE and oppositeDirection(leftDir, directions[row][col])) {
                withinTrench = false;
            } else if (space[row][col] == TRENCH and leftDir != NONE and sameDirection(leftDir, directions[row][col])) {
                withinTrench = true;
            }
            if (space[row][col] == TRENCH) {
                std::cout << "Adding trench for " << row << ", " << col << "\n";
                trenchSize++;
                if (leftDir == NONE) {
                    setLeftDir(&leftDir, directions[row][col]);
                    withinTrench = true;
                }
            } else if (space[row][col] == EMPTY_SPACE and withinTrench) {
                std::cout << "Adding empty space for " << row << ", " << col << "\n";                
                trenchSize++;
            }
        }
    }
    return trenchSize;
}

bool oppositeDirection(direction leftDir, std::vector<direction> dirs) {
    for (direction d : dirs) {
        if (leftDir == UP and d == DOWN)
            return true;
        else if (leftDir == DOWN and d == UP)
            return true;
    }
    return false;
}

bool sameDirection(direction leftDir, std::vector<direction> dirs) {
    for (direction d : dirs) {
        if (leftDir == d)
            return true;
    }
    return false;
}

void setLeftDir(direction* leftDir, std::vector<direction> dirs) {
    for (direction d : dirs) {
        if (d == UP or d == DOWN) {
            *leftDir = d;
            break;
        }
    }
}