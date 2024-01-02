#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#define OBSTACLE '#'
#define LEFT_SLOPE '<'
#define RIGHT_SLOPE '>'
#define UP_SLOPE '^'
#define DOWN_SLOPE 'v'
#define ENDING_INDEX 1

std::vector<std::vector<char>> graph = {};
std::pair<int, int> startingCoordinates;
std::pair<int, int> endingCoordinates;
int numRows;
int numCols;

bool checkIfJunction(std::pair<int, int> coordinates);
bool checkIfObstacle(std::pair<int, int> coordinates);

struct junction {
    std::pair<int, int> coordinates;
    std::vector<std::pair<std::pair<int, int>, long long>> connectedJunctions;

    junction(std::pair<int, int> coordinates) {
        this->coordinates = coordinates;
        connectedJunctions = {};
    }

    void findJunctions() {
        std::stack<std::pair<std::pair<int, int>, long long>> frontier = {};
        std::map<std::pair<int, int>, int> visited = {};
        visited[coordinates] = 1;
        frontier.push({{coordinates.first - 1, coordinates.second}, 1});
        frontier.push({{coordinates.first + 1, coordinates.second}, 1});
        frontier.push({{coordinates.first, coordinates.second - 1}, 1});
        frontier.push({{coordinates.first, coordinates.second + 1}, 1});
        while (!frontier.empty()) {
            std::pair<std::pair<int, int>, long long> curr = frontier.top();
            frontier.pop();
            if (visited.count(curr.first))
                continue;
            visited[curr.first] = 1;
            if (curr.first.first < 0 or curr.first.first >= numRows or curr.first.second < 0 or curr.first.second >= numCols)
                continue;
            if (checkIfObstacle(curr.first))
                continue;
            if (checkIfJunction(curr.first)) {
                std::cout << "Connected junction to " << coordinates.first << ", " << coordinates.second << " is coordinates: " << curr.first.first << ", " << curr.first.second << "\n";
                std::cout << "Steps needed is: " << curr.second << "\n";
                connectedJunctions.push_back(curr);
                continue;
            }
            frontier.push({{curr.first.first - 1, curr.first.second}, curr.second + 1});
            frontier.push({{curr.first.first + 1, curr.first.second}, curr.second + 1});
            frontier.push({{curr.first.first, curr.first.second - 1}, curr.second + 1});
            frontier.push({{curr.first.first, curr.first.second + 1}, curr.second + 1});
        }
        std::cout << connectedJunctions.size() << " connected junctions\n";
    }
};

void processLine(std::string s);
long long longestPath();
void findJunctions();

std::map<std::pair<int, int>, junction*> junctions = {};

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
    
    findJunctions();
    std::cout << "All junctions found\n";

    for (std::map<std::pair<int, int>, junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it) {
        (it->second)->findJunctions();
    }
    std::cout << "All junctions connected\n";
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

bool checkIfJunction(std::pair<int, int> coordinates) {
    if (coordinates.first == startingCoordinates.first and coordinates.second == startingCoordinates.second)
        return true;
    if (coordinates.first == endingCoordinates.first and coordinates.second == endingCoordinates.second)
        return true;
    if (checkIfObstacle(coordinates))
        return false;
    int numAdjacent = 0;
    if (coordinates.first > 0 and !checkIfObstacle({coordinates.first - 1, coordinates.second}))
        numAdjacent++;
    if (coordinates.first < numRows - 1 and !checkIfObstacle({coordinates.first + 1, coordinates.second}))
        numAdjacent++;
    if (coordinates.second > 0 and !checkIfObstacle({coordinates.first, coordinates.second - 1}))
        numAdjacent++;
    if (coordinates.second < numCols - 1 and !checkIfObstacle({coordinates.first, coordinates.second + 1}))
        numAdjacent++;
    return numAdjacent > 2;
}

void findJunctions() {
    int numJunctions = 0;
    for (int row = 0; row < numRows; row++)
        for (int col = 0; col < numCols; col++)
            if (checkIfJunction({row, col})) {
                std::cout << "Junction found: " << row << ", " << col << "\n";
                junctions[{row, col}] = new junction({row, col});
                numJunctions++;
            }
    std::cout << numJunctions << " junctions found\n";
}

struct movement {
    junction* currJunction;
    std::map<std::pair<int, int>, int> visitedJunctions;
    long long currPathLength;

    movement(junction* currJunction, std::map<std::pair<int, int>, int> visitedJunctions, long long currPathLength) {
        this->currJunction = currJunction;
        this->visitedJunctions = visitedJunctions;
        this->visitedJunctions[currJunction->coordinates] = 1;
        this->currPathLength = currPathLength;
    }

    void updateFrontier(std::stack<movement>& frontier) {
        for (std::pair<std::pair<int, int>, long long> p : currJunction->connectedJunctions) {
            if (!this->visitedJunctions.count(p.first))
                frontier.push(movement(junctions[p.first], visitedJunctions, currPathLength + p.second));
        }
    }
};

// more efficient longest path algorithm that does dfs through the junctions instead
long long longestPath() {
    long long longestLen = 0;
    std::stack<movement> frontier = {};
    frontier.push(movement(junctions[startingCoordinates], {}, 0));
    while (!frontier.empty()) {
        movement currMovement = frontier.top();
        frontier.pop();
        if (currMovement.currJunction->coordinates == endingCoordinates) {
            longestLen = std::max(longestLen, currMovement.currPathLength);
            std::cout << "New longest len: " << longestLen << "\n";
            continue;
        }
        currMovement.updateFrontier(frontier);
    }
    return longestLen;
}