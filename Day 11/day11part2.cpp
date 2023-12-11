#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

#define GALAXY '#'
#define EXPANSION_DISTANCE 1000000

void fillRowOfGraph(std::string s);
void expandGraph();
bool checkColumn(int col);
bool checkRow(int row);
void findGalaxies();
long long findSumPair();
long long bfs(std::map<std::pair<int, int>, int> const& ignoredPoints, std::pair<int, int> startingPoint);

struct point {
    bool isGalaxy;
    int additionalHorizontalDistance = 0;
    int additionalVerticalDistance = 0;

    point(char c) {
        isGalaxy = c == GALAXY;
    }

    void addAdditionalHorizontalDistance() {
        additionalHorizontalDistance = EXPANSION_DISTANCE - 1;
    }

    void addAdditionalVerticalDistance() {
        additionalVerticalDistance = EXPANSION_DISTANCE - 1;
    }
};

std::vector<std::vector<point>> graph = {};
int m;
int n;
std::vector<std::pair<int, int>> galaxyCoordinates = {};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
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
            fillRowOfGraph(s);
        }
    }
    m = graph.size();
    n = graph[0].size();
    expandGraph();
    findGalaxies();
    std::cout << findSumPair() << "\n";
}

void fillRowOfGraph(std::string s) {
    std::vector<point> row = {};
    for (char c : s) {
        row.push_back(point(c));
    }
    graph.push_back(row);
}

bool checkRow(int row) {
    for (point p : graph[row]) {
        if (p.isGalaxy)
            return false;
    }
    return true;
}

bool checkColumn(int col) {
    for (int row = 0; row < m; row++) {
        if (graph[row][col].isGalaxy)
            return false;
    }
    return true;
}

void expandGraph() {
    std::vector<int> rowsToExpand = {};
    for (int row = 0; row < m; row++) {
        if (checkRow(row)) {
            for (int col = 0; col < n; col++) {
                graph[row][col].addAdditionalVerticalDistance();
            }
        }
    }
    for (int col = 0; col < n; col++) {
        if (checkColumn(col)) {
            for (int row = 0; row < m; row++)
                graph[row][col].addAdditionalHorizontalDistance();
        }
    }
}

void findGalaxies() {
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (graph[row][col].isGalaxy) {
                galaxyCoordinates.push_back({row, col});
            }
        }
    }
}

long long findSumPair() {
    std::map<std::pair<int, int>, int> ignoredPoints = {};
    long long sum = 0;
    for (std::pair<int, int> galaxy : galaxyCoordinates) {
        sum += bfs(ignoredPoints, galaxy);
        ignoredPoints[galaxy] = 1;
    }
    return sum;
}

long long bfs(std::map<std::pair<int, int>, int> const& ignoredPoints, std::pair<int, int> startingPoint) {
    std::deque<std::pair<std::pair<int, int>, long long>> frontier = {};
    std::map<std::pair<int, int>, int> visitedPoints = {};
    frontier.push_back({startingPoint, 0});
    long long sum = 0;
    while (!frontier.empty()) {
        std::pair<std::pair<int, int>, int> temp = frontier.front();
        frontier.pop_front();
        std::pair<int, int> coordinates = temp.first;
        long long length = temp.second;
        if (visitedPoints.count(coordinates))
            continue;
        if (graph[coordinates.first][coordinates.second].isGalaxy and !ignoredPoints.count(coordinates))
            sum += length;
        visitedPoints[coordinates] = 1;
        if (coordinates.first > 0)
            frontier.push_back({{coordinates.first - 1, coordinates.second}, length + 1 + graph[coordinates.first][coordinates.second].additionalVerticalDistance});
        if (coordinates.first < m - 1)
            frontier.push_back({{coordinates.first + 1, coordinates.second}, length + 1 + graph[coordinates.first][coordinates.second].additionalVerticalDistance});
        if (coordinates.second > 0)
            frontier.push_back({{coordinates.first, coordinates.second - 1}, length + 1 + graph[coordinates.first][coordinates.second].additionalHorizontalDistance});
        if (coordinates.second < n - 1)
            frontier.push_back({{coordinates.first, coordinates.second + 1}, length + 1 + graph[coordinates.first][coordinates.second].additionalHorizontalDistance});
    }
    return sum;
}