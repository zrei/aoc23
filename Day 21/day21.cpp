#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define STARTING_POINT 'S'
#define OBSTACLE '#'
#define PATH_LENGTH 64

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
long long bfs();

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
    std::cout << bfs() << "\n";
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
    return graph[coordinates.first][coordinates.second] == OBSTACLE;
}

long long bfs() {
    long long numGrids = 0;
    std::deque<node> frontier = {node(startingCoordinates, 0)};
    std::map<std::pair<int, std::pair<int, int>>, int> encounteredFinalGrids = {};
    while (!frontier.empty()) {
        node currNode = frontier.front();
        frontier.pop_front();
        std::pair<int, int> currCoordinates = currNode.coordinates;
        if (currCoordinates.first < 0 or currCoordinates.first >= numRow or currCoordinates.second < 0 or currCoordinates.second >= numCol)
            continue;
        if (checkIfObstacle(currCoordinates))
            continue;
        if (encounteredFinalGrids.count({currNode.pathSize, currNode.coordinates}))
            continue;
        encounteredFinalGrids[{currNode.pathSize, currNode.coordinates}] = 1;
        if (currNode.pathSize == PATH_LENGTH) {
            std::cout << currCoordinates.first << ", " << currCoordinates.second << "\n";
            numGrids++;
            continue;
        }
        frontier.push_back(currNode.stepDown());
        frontier.push_back(currNode.stepLeft());
        frontier.push_back(currNode.stepRight());
        frontier.push_back(currNode.stepUp());
    }
    return numGrids;
}