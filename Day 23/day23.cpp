#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#define OBSTACLE '#'
#define LEFT_SLOPE '<'
#define RIGHT_SLOPE '>'
#define UP_SLOPE '^'
#define DOWN_SLOPE 'v'

std::vector<std::vector<char>> graph = {};
std::pair<int, int> startingCoordinates;
std::pair<int, int> endingCoordinates;
int numRows;
int numCols;

enum direction {
    LEFT,
    RIGHT,
    DOWN,
    UP
};

struct movement {
    direction entranceDirection;
    std::pair<int, int> coordinates;
    long long pathLength;
    std::map<std::pair<int, int>, int> visitedPoints;

    movement(direction entranceDirection, std::pair<int, int> coordinates, long long pathLength, std::map<std::pair<int, int>, int> encounteredPoints) {
        this->entranceDirection = entranceDirection;
        this->coordinates = coordinates;
        this->pathLength = pathLength;
        this->visitedPoints = encounteredPoints;
        visitedPoints[coordinates] = 1;
    }

    void move(std::deque<movement>& frontier) {
        if (entranceDirection != LEFT) {
            moveRight(frontier);
        }
        if (entranceDirection != RIGHT) {
            moveLeft(frontier);
        }
        if (entranceDirection != UP) {
            moveDown(frontier);
        }
        if (entranceDirection != DOWN) {
            moveUp(frontier);
        }
    }

    void moveLeft(std::deque<movement>& frontier) {
        if (visitedPoints.count({coordinates.first, coordinates.second - 1}))
            return;
        frontier.push_back(movement(LEFT, {coordinates.first, coordinates.second - 1}, pathLength + 1, visitedPoints));
    }

    void moveRight(std::deque<movement>& frontier) {
        if (visitedPoints.count({coordinates.first, coordinates.second + 1}))
            return;
        frontier.push_back(movement(RIGHT, {coordinates.first, coordinates.second + 1}, pathLength + 1, visitedPoints));
    }

    void moveUp(std::deque<movement>& frontier) {
        if (visitedPoints.count({coordinates.first - 1, coordinates.second}))
            return;
        frontier.push_back(movement(UP, {coordinates.first - 1, coordinates.second}, pathLength + 1, visitedPoints));
    }

    void moveDown(std::deque<movement>& frontier) {
        if (visitedPoints.count({coordinates.first + 1, coordinates.second}))
            return;
        frontier.push_back(movement(DOWN, {coordinates.first + 1, coordinates.second}, pathLength + 1, visitedPoints));
    }

    void moveSlope(direction dir, std::deque<movement>& frontier) {
        switch (dir) {
            case UP:
                if (entranceDirection == DOWN)
                    return;
                moveUp(frontier);
            case DOWN:
                if (entranceDirection == UP)
                    return;
                moveDown(frontier);
            case RIGHT:
                if (entranceDirection == LEFT)
                    return;
                moveRight(frontier);
            case LEFT:
                if (entranceDirection == RIGHT)
                    return;
                moveUp(frontier);
        }
    }
};

void processLine(std::string s);
bool checkIfObstacle(std::pair<int, int> coordinates);
bool checkIfSlope(std::pair<int, int> coordinates);
direction getSlopeDirection(std::pair<int, int> coordinates);
long long longestPath();

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
    numRows = graph.size();
    numCols = graph[0].size();

    startingCoordinates = {0, 1};
    endingCoordinates = {numRows - 1, numCols - 1 - 1};
    std::cout << "Rows, cols: " << numRows << ", " << numCols << "\n";
    std::cout << "Starting coordinate: " << startingCoordinates.first << ", " << startingCoordinates.second << "\n";
    std::cout << "Ending coordinate: " << endingCoordinates.first << ", " << endingCoordinates.second << "\n";
    std::cout << longestPath() << "\n";
}

void processLine(std::string s) {
    std::vector<char> row = {};
    for (char i : s) {
        row.push_back(i);
    }
    graph.push_back(row);
}

bool checkIfObstacle(std::pair<int, int> coordinates) {
    return graph[coordinates.first][coordinates.second] == OBSTACLE;
}

bool checkIfSlope(std::pair<int, int> coordinates) {
    char tile = graph[coordinates.first][coordinates.second];
    return tile == DOWN_SLOPE or tile == UP_SLOPE or tile == RIGHT_SLOPE or tile == DOWN_SLOPE;
}

direction getSlopeDirection(std::pair<int, int> coordinates) {
    char tile = graph[coordinates.first][coordinates.second];
    switch (tile) {
        case LEFT_SLOPE:
            return LEFT;
        case RIGHT_SLOPE:
            return RIGHT;
        case UP_SLOPE:
            return UP;
        case DOWN_SLOPE:
            return DOWN;
    }
    return UP;
}

long long longestPath() {
    std::deque<movement> frontier = {};
    std::map<std::pair<int, int>, int> temp = {};
    temp[startingCoordinates] = 1;
    frontier.push_back(movement(UP, {startingCoordinates.first - 1, startingCoordinates.second}, 1, temp));
    frontier.push_back(movement(DOWN, {startingCoordinates.first + 1, startingCoordinates.second}, 1, temp));
    frontier.push_back(movement(RIGHT, {startingCoordinates.first, startingCoordinates.second + 1}, 1, temp));
    frontier.push_back(movement(LEFT, {startingCoordinates.first, startingCoordinates.second - 1}, 1, temp));
    long long longestLen = 0;
    while (!frontier.empty()) {
        movement curr = frontier.front();
        frontier.pop_front();
        if (curr.coordinates.first < 0 or curr.coordinates.first >= numRows or curr.coordinates.second < 0 or curr.coordinates.second >= numCols) {
            continue;
        }
        if (checkIfObstacle(curr.coordinates)) {
            continue;
        }
        if (curr.coordinates.first == endingCoordinates.first and curr.coordinates.second == endingCoordinates.second) {
            longestLen = std::max(longestLen, curr.pathLength);
            std::cout << "New longest len: " << longestLen << "\n";
            continue;
        }
        if (checkIfSlope(curr.coordinates)) {
            curr.moveSlope(getSlopeDirection(curr.coordinates), frontier);
            continue;
        }
        curr.move(frontier);
    }
    return longestLen;
}