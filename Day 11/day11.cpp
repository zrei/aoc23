#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

#define GALAXY '#'

void fillRowOfGraph(std::string s);
void expandGraph();
bool checkColumn(int col);
bool checkRow(int row);
void findGalaxies();
int findSumPair();
int bfs(std::map<std::pair<int, int>, int> const& ignoredPoints, std::pair<int, int> startingPoint);
void printGraph();

struct point {
    bool isGalaxy;

    point(char c) {
        isGalaxy = c == GALAXY;
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
    //findGalaxies();
    //galaxyCoordinates.clear();
    expandGraph();
    findGalaxies();
    printGraph();
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
    for (int row = 0; row < graph.size(); row++) {
        if (graph[row][col].isGalaxy)
            return false;
    }
    return true;
}

void expandGraph() {
    std::vector<int> rowsToExpand = {};
    for (int row = 0; row < graph.size(); row++) {
        if (checkRow(row)) {
            rowsToExpand.push_back(row);
        }
    }
    std::vector<std::vector<point>> temp = {};
    int i = 0;
    for (int row = 0; row < graph.size(); row++) {
        if (i < rowsToExpand.size() and row == rowsToExpand[i]) {
            temp.push_back(graph[row]);
            temp.push_back(graph[row]);
            i++;
        } else {
            temp.push_back(graph[row]);
        }
    }
    graph = temp;
    std::vector<int> colsToExpand = {};
    for (int col = 0; col < graph[0].size(); col++) {
        if (checkColumn(col)) {
            colsToExpand.push_back(col);
        }
    }
    temp.clear();
    for (int row = 0; row < graph.size(); row++) {
        i = 0;
        temp.push_back({});
        for (int col = 0; col < graph[0].size(); col++) {
            if (i < colsToExpand.size() and col == colsToExpand[i]) {
                temp[row].push_back(graph[row][col]);
                temp[row].push_back(graph[row][col]);
                i++;
            }
            else
                temp[row].push_back(graph[row][col]);
        }
    }
    graph = temp;
    m = graph.size();
    n = graph[0].size();
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

int findSumPair() {
    std::map<std::pair<int, int>, int> ignoredPoints = {};
    int sum = 0;
    for (std::pair<int, int> galaxy : galaxyCoordinates) {
        sum += bfs(ignoredPoints, galaxy);
        ignoredPoints[galaxy] = 1;
    }
    return sum;
}

int bfs(std::map<std::pair<int, int>, int> const& ignoredPoints, std::pair<int, int> startingPoint) {
    std::deque<std::pair<std::pair<int, int>, int>> frontier = {};
    std::map<std::pair<int, int>, int> visitedPoints = {};
    frontier.push_back({startingPoint, 0});
    int sum = 0;
    int numPairs = 0;
    while (!frontier.empty()) {
        std::pair<std::pair<int, int>, int> temp = frontier.front();
        frontier.pop_front();
        std::pair<int, int> coordinates = temp.first;
        int length = temp.second;
        if (visitedPoints.count(coordinates))
            continue;
        if (graph[coordinates.first][coordinates.second].isGalaxy and !ignoredPoints.count(coordinates)) {
            sum += length;
            numPairs++;
        }
        visitedPoints[coordinates] = 1;
        if (coordinates.first > 0)
            frontier.push_back({{coordinates.first - 1, coordinates.second}, length + 1});
        if (coordinates.first < m - 1)
            frontier.push_back({{coordinates.first + 1, coordinates.second}, length + 1});
        if (coordinates.second > 0)
            frontier.push_back({{coordinates.first, coordinates.second - 1}, length + 1});
        if (coordinates.second < n - 1)
            frontier.push_back({{coordinates.first, coordinates.second + 1}, length + 1});
    }
    return sum;
}

void printGraph() {
    std::ofstream writer;
    writer.open("output.txt");
    for (int row = 0; row < graph.size(); row++) {
        for (int col = 0; col < graph[0].size(); col++) {
            writer << (graph[row][col].isGalaxy ? '#' : '.');
        }
        writer << "\n";
    }
    writer.close();
}