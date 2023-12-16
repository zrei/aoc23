#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#define EMPTY_SPACE '.'
#define RIGHT_SLANT_MIRROR '/'
#define LEFT_SLANT_MIRROR '\\'
#define HORIZONTAL_SPLITTER '-'
#define VERTICAL_SPLITTER '|'

enum direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

std::map<std::pair<std::pair<int, int>, direction>, int> encountered = {};
std::vector<std::string> graph = {};
int numEnergised = 0;
int maxNumEnergised = 0;
std::map<std::pair<int, int>, int> energised = {};
int numRows;
int numCols;

void bfs(std::pair<int, int> startingCoord, direction startingDir);
void updateEnergised(std::pair<int, int> coordinates);
std::vector<std::pair<std::pair<int, int>, direction>> beamResult(std::pair<int, int> coordinates, direction initialDir);
char getGraphTile(std::pair<int, int> coordinates);
std::vector<direction> processRightSlantMirror(direction initialDir);
std::vector<direction> processLeftSlantMirror(direction initialDir);
std::vector<direction> processHorizontalSplitter(direction initialDir);
std::vector<direction> processVerticalSpliter(direction initialDir);
std::vector<std::pair<std::pair<int, int>, direction>> newPoints(std::pair<int, int> initialCoordinates, std::vector<direction> directions);
void goInDirection(std::pair<int, int> initialCoordinates, direction dir, std::vector<std::pair<std::pair<int, int>, direction>>& newPoints);

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
    
    std::cout << numRows << ", " << numCols << "\n";
    for (int col = 0; col < numCols; col++) {
        bfs({0, col}, DOWN);
        bfs({numRows - 1, col}, UP);        
    }

    for (int row = 0; row < numRows; row++) {
        bfs({row, 0}, RIGHT);
        bfs({row, numCols - 1}, LEFT);
    }

    std::cout << maxNumEnergised << "\n";
}

void bfs(std::pair<int, int> startingCoord, direction startingDir) {
    encountered.clear();
    energised.clear();
    numEnergised = 0;
    std::pair<std::pair<int, int>, direction> initialPoint = {startingCoord, startingDir};
    std::deque frontier = {initialPoint};
    while (!frontier.empty()) {
        std::pair<std::pair<int, int>, direction> currPoint = frontier.front();
        frontier.pop_front();
        if (encountered.count(currPoint))
            continue;
        encountered[currPoint] = 1;
        updateEnergised(currPoint.first);
        std::vector<std::pair<std::pair<int, int>, direction>> newPoints = beamResult(currPoint.first, currPoint.second);
        for (std::pair<std::pair<int, int>, direction> p : newPoints)
            frontier.push_back(p);
    }
    maxNumEnergised = std::max(maxNumEnergised, numEnergised);
}

void updateEnergised(std::pair<int, int> coordinates) {
    if (energised.count(coordinates))
        return;
    numEnergised++;
    energised[coordinates] = 1;
}

std::vector<std::pair<std::pair<int, int>, direction>> beamResult(std::pair<int, int> coordinates, direction initialDir) {
    char c = getGraphTile(coordinates);
    switch (c) {
        case EMPTY_SPACE:
            return newPoints(coordinates, {initialDir});
        case RIGHT_SLANT_MIRROR:
            return newPoints(coordinates, processRightSlantMirror(initialDir));
        case LEFT_SLANT_MIRROR:
            return newPoints(coordinates, processLeftSlantMirror(initialDir));
        case HORIZONTAL_SPLITTER:
            return newPoints(coordinates, processHorizontalSplitter(initialDir));
        case VERTICAL_SPLITTER:
            return newPoints(coordinates, processVerticalSpliter(initialDir));
    }
    return {};
}

char getGraphTile(std::pair<int, int> coordinates) {
    return graph[coordinates.first][coordinates.second];
}

std::vector<direction> processRightSlantMirror(direction initialDir) {
    switch (initialDir) {
        case RIGHT:
            return {UP};
        case LEFT:
            return {DOWN};
        case UP:
            return {RIGHT};
        case DOWN:
            return {LEFT};
    }
    return {};
}

std::vector<direction> processLeftSlantMirror(direction initialDir) {
    switch (initialDir) {
        case RIGHT:
            return {DOWN};
        case LEFT:
            return {UP};
        case UP:
            return {LEFT};
        case DOWN:
            return {RIGHT};
    }
    return {};
}

std::vector<direction> processHorizontalSplitter(direction initialDir) {
    switch (initialDir) {
        case RIGHT:
            return {RIGHT};
        case LEFT:
            return {LEFT};
        case UP:
        case DOWN:
            return {LEFT, RIGHT};
    }
    return {};
}

std::vector<direction> processVerticalSpliter(direction initialDir) {
    switch (initialDir) {
        case UP:
            return {UP};
        case DOWN:
            return {DOWN};
        case LEFT:
        case RIGHT:
            return {UP, DOWN};
    }
    return {};
}

std::vector<std::pair<std::pair<int, int>, direction>> newPoints(std::pair<int, int> initialCoordinates, std::vector<direction> directions) {
    std::vector<std::pair<std::pair<int, int>, direction>> points = {};
    for (direction d : directions) {
        goInDirection(initialCoordinates, d, points);
    }
    return points;
}

void goInDirection(std::pair<int, int> initialCoordinates, direction dir, std::vector<std::pair<std::pair<int, int>, direction>>& newPoints) {
    switch (dir) {
        case LEFT:
            if (initialCoordinates.second > 0)
                newPoints.push_back({{initialCoordinates.first, initialCoordinates.second - 1}, dir});
            break;
        case RIGHT:
            if (initialCoordinates.second < numCols - 1)
                newPoints.push_back({{initialCoordinates.first, initialCoordinates.second + 1}, dir});
            break;
        case UP:
            if (initialCoordinates.first > 0)
                newPoints.push_back({{initialCoordinates.first - 1, initialCoordinates.second}, dir});
            break;
        case DOWN:
            if (initialCoordinates.first < numRows - 1)
                newPoints.push_back({{initialCoordinates.first + 1, initialCoordinates.second}, dir});
            break;
    }
}