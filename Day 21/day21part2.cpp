#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>

#define STARTING_POINT 'S'
#define OBSTACLE '#'
#define PATH_LENGTH 26501365

struct node {
    std::pair<int, int> coordinates;
    long long pathSize;

    node(std::pair<int, int> coordinates, long long pathSize) {
        this->coordinates = coordinates;
        this->pathSize = pathSize;
    }

    node stepLeft() {
        return node({coordinates.first, coordinates.second - 1}, pathSize + 1);
    }

    node stepRight() {
        return node({coordinates.first, coordinates.second + 1}, pathSize + 1);
    }

    node stepUp() {
        return node({coordinates.first - 1, coordinates.second}, pathSize + 1);
    }

    node stepDown() {
        return node({coordinates.first + 1, coordinates.second}, pathSize + 1);
    }
};

std::vector<std::vector<char>> graph = {};
std::pair<int, int> startingCoordinates = {0, 0};
bool startingPointFound = false;
int numRow;
int numCol;

void processLine(std::string s);
bool checkIfObstacle(std::pair<int, int> coordinates);
long long bfs(long long size);
long long quadratic(long long y1, long long y2, long long y3, long long n);

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
    numRow = graph.size();
    numCol = graph[0].size();
    std::cout << startingCoordinates.first << ", " << startingCoordinates.second << ", " << numRow << ", " << numCol << "\n";

    long long edge = numCol / 2;
    long long y1 = bfs(edge);
    long long y2 = bfs(edge + 1 * numCol);
    long long y3 = bfs(edge + 2 * numCol);

    std::cout << y1 << ", " << y2 << ", " << y3 << "\n";
    
    std::cout << quadratic(y1, y2, y3, ((PATH_LENGTH - edge) / numCol)) << "\n";
}

void processLine(std::string s) {
    std::vector<char> temp = {};
    for (char i : s) {
        if (!startingPointFound and i == STARTING_POINT) {
            startingPointFound = true;
        }
        temp.push_back(i);
        if (!startingPointFound)
            startingCoordinates.second++;
    }
    graph.push_back(temp);
    if (!startingPointFound)
        startingCoordinates = {startingCoordinates.first + 1, 0};
}

bool checkIfObstacle(std::pair<int, int> coordinates) {
    while (coordinates.first < 0) // deal with cpp's different behaviour for modulus involving negative numbers
        coordinates.first += 131;
    while (coordinates.second < 0)
        coordinates.second += 131;
    return graph[coordinates.first % 131][coordinates.second % 131] == OBSTACLE; // the modulus is to deal with the infinite plot
}

long long bfs(long long size) {
    // simplified bfs compared to part 1: when reaching a point, include it in reachable points if the number of remaining steps is even
    long long numGrids = 0;
    std::deque<node> frontier = {node(startingCoordinates, 0)};
    std::map<std::pair<int, int>, int> encounteredGrids = {};
    while (!frontier.empty()) {
        node currNode = frontier.front();
        frontier.pop_front();
        std::pair<int, int> currCoordinates = currNode.coordinates;
        if (encounteredGrids.count({currCoordinates}))
            continue;
        encounteredGrids[currCoordinates] = 1;
        if (checkIfObstacle(currCoordinates))
            continue;
        if ((size - currNode.pathSize) % 2 == 0)
            numGrids++;
        if (currNode.pathSize == size) {
            continue;
        }
        frontier.push_back(currNode.stepDown());
        frontier.push_back(currNode.stepLeft());
        frontier.push_back(currNode.stepRight());
        frontier.push_back(currNode.stepUp());
    }
    return numGrids;
}

long long quadratic(long long y1, long long y2, long long y3, long long n) {
    // Use the quadratic formula to find the output at the large steps based on the first three data points
    long long a = (y3 - (2 * y2) + y1) / 2;
    long long b = y2 - y1 - a;
    long long c = y1;
    return (a * std::pow(n, 2)) + (b * n) + c;
}
