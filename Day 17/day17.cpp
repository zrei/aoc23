#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct node {
    std::pair<int, int> coordinates;
    direction dir;
    int pathLength;
    long long cost;

    node(std::pair<int, int> c, direction d, int p, long long co) {
        coordinates = c;
        dir = d;
        pathLength = p;
        cost = co;
    }

    bool operator<(const node &o)  const {
        if (coordinates.first < o.coordinates.first) return true;
        else if (coordinates.first > o.coordinates.first) return false;
        else if (coordinates.second < o.coordinates.second) return true;
        else if (coordinates.second > o.coordinates.second) return false;
        else if (dir < o.dir) return true;
        else if (dir > o.dir) return false;
        else if (pathLength < o.pathLength) return true;
        else return false;
    }
};

struct customComparator {
    bool operator()(const node l, const node r) const { return -l.cost < -r.cost; }
};

std::vector<std::string> graph = {};
std::priority_queue<node, std::vector<node>, customComparator> frontier = {};
std::map<node, int> encountered = {};
int numRows;
int numCols;

int getTileCost(std::pair<int, int> coordinates);
long long dijkstra();
std::vector<direction> ninetyDegreeTurn(direction d);
std::pair<int, int> getNewCoordinates(std::pair<int, int> initialCoordinates, direction dir);
void updateFrontier(std::pair<int, int> initialCoordinates, std::vector<direction> dirs, direction currDir, int currPathLength, long long currCost, std::priority_queue<node, std::vector<node>, customComparator>& frontier);

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
            graph.push_back(s);
        }
    }
    numRows = graph.size();
    numCols = graph[0].size();

    std::cout << dijkstra() << "\n";
}

int getTileCost(std::pair<int, int> coordinates) {
    return graph[coordinates.first][coordinates.second] - '0';
}

long long dijkstra() {
    node firstNode = node({1, 0}, DOWN, 1, getTileCost({1, 0}));
    frontier.push(firstNode);
    node secondNode = node({0, 1}, RIGHT, 1, getTileCost({0, 1}));
    frontier.push(secondNode);
    while (!frontier.empty()) {
        node currNode = frontier.top();
        std::cout << "Curr node: " << currNode.coordinates.first << ", " << currNode.coordinates.second << "\n";
        std::cout << "Cost: " << currNode.cost << "\n";
        frontier.pop();
        if (currNode.coordinates.first == numRows - 1 and currNode.coordinates.second == numCols - 1)
            return currNode.cost;
        if (encountered.count(currNode))
            continue;
        encountered[currNode] = 1;
        if (currNode.pathLength == 3) {
            updateFrontier(currNode.coordinates, ninetyDegreeTurn(currNode.dir), currNode.dir, currNode.pathLength, currNode.cost, frontier);
        } else {
            std::vector<direction> dirs = ninetyDegreeTurn(currNode.dir);
            dirs.push_back(currNode.dir);
            updateFrontier(currNode.coordinates, dirs, currNode.dir, currNode.pathLength, currNode.cost, frontier);
        }
    }
    return 0;
}

std::vector<direction> ninetyDegreeTurn(direction d) {
    switch (d) {
        case UP:
        case DOWN:
            return {LEFT, RIGHT};
        case LEFT:
        case RIGHT:
            return {UP, DOWN};
    }
    return {};
}

std::pair<int, int> getNewCoordinates(std::pair<int, int> initialCoordinates, direction dir) {
    switch (dir) {
        case UP:
            return {initialCoordinates.first - 1, initialCoordinates.second};
        case DOWN:
            return {initialCoordinates.first + 1, initialCoordinates.second};
        case LEFT:
            return {initialCoordinates.first, initialCoordinates.second - 1};
        case RIGHT:
            return {initialCoordinates.first, initialCoordinates.second + 1};
    }
    return {0, 0};
}

void updateFrontier(std::pair<int, int> initialCoordinates, std::vector<direction> dirs, direction currDir, int currPathLength, long long currCost, std::priority_queue<node, std::vector<node>, customComparator>& frontier) {
    for (direction dir : dirs) {
        std::pair<int, int> newCoordinates = getNewCoordinates(initialCoordinates, dir);
        if (newCoordinates.first < 0 or newCoordinates.first >= numRows or newCoordinates.second < 0 or newCoordinates.second >= numCols) 
            continue;
        if (currDir == dir)
            frontier.push(node(newCoordinates, dir, currPathLength + 1, currCost + getTileCost(newCoordinates)));
        else
            frontier.push(node(newCoordinates, dir, 1, currCost + getTileCost(newCoordinates)));
    }
}