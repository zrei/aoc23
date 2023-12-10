#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#define STARTING_POINT 'S'
#define VERTICAL '|'
#define HORIZONTAL '-'
#define NORTH_EAST 'L'
#define NORTH_WEST 'J'
#define SOUTH_WEST '7'
#define SOUTH_EAST 'F'
#define GROUND '.'
#define COLUMN_EXPANSION '*'
#define ROW_EXPANSION '?'
#define PADDING '!'

enum direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NONE
};

struct point {
    std::pair<int, int> coordinates;
    direction entranceDirection;

    point(std::pair<int, int> c, direction e) {
        coordinates = c;
        entranceDirection = e;
    }
};

std::vector<std::string> graph = {};
std::vector<std::vector<char>> expandedGraph = {};
std::pair<int, int> startingPoint;
std::deque<point> frontier = {};
std::map<std::pair<int, int>, int> pathPoints = {};
std::map<std::pair<int, int>, int> exploredIslandPoints = {};
std::map<std::pair<int, int>, int> islandPoints = {};
int m;
int n;

void fillGraph(std::string s);
void findStartingPoint();
bool isOpposite(direction firstDir, direction secondDir);
char getTile(std::pair<int, int> coordinates);
std::pair<direction, direction> getJoiningDirections(char c);
std::pair<int, int> getNewCoordinates(std::pair<int, int> oldCoordinates, direction dir);
bool isOpposite(direction firstDir, direction secondDir);
void findPath();
int findingIslands();
int numTilesInIsland(std::pair<int, int> startingPoint);
void printPath();
void expandResolution();
void printExpandedGraph();
bool allowVertical(char left, char right);
bool allowHorizontal(char top, char bottom);
bool returnCanPassThrough(std::pair<int, int> coordinate);

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
            fillGraph(s);
        }
    }
    expandResolution();
    printExpandedGraph();
    findStartingPoint();
    findPath();
    printPath();
    //findStartingPoint();
    //findPath();
    //printGraph();
    //std::cout << findingIslands() << "\n";
}

void fillGraph(std::string s) {
    graph.push_back(s);
}

void findStartingPoint() {
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (expandedGraph[row][col] == STARTING_POINT) {
                startingPoint = {row, col};
                return;
            }
        }
    }
}

char getTile(std::pair<int, int> coordinates) {
    return expandedGraph[coordinates.first][coordinates.second];
}

std::pair<direction, direction> getJoiningDirections(char c) {
    switch (c) {
        case VERTICAL:
            return {NORTH, SOUTH};
        case HORIZONTAL:
            return {EAST, WEST};
        case NORTH_EAST:
            return {NORTH, EAST};
        case NORTH_WEST:
            return {NORTH, WEST};
        case SOUTH_EAST:
            return {SOUTH, EAST};
        case SOUTH_WEST:
            return {SOUTH, WEST};
        default:
            return {NORTH, NORTH};
    }
}

std::pair<int, int> getNewCoordinates(std::pair<int, int> oldCoordinates, direction dir) {
    switch (dir) {
        case NORTH:
            return {oldCoordinates.first - 1, oldCoordinates.second};
        case SOUTH:
            return {oldCoordinates.first + 1, oldCoordinates.second};
        case WEST:
            return {oldCoordinates.first, oldCoordinates.second - 1};
        case EAST:
            return {oldCoordinates.first, oldCoordinates.second + 1};
        default:
            return oldCoordinates;
    }
}

bool isOpposite(direction firstDir, direction secondDir) {
    switch (firstDir) {
        case NORTH:
            return secondDir == SOUTH;
        case SOUTH:
            return secondDir == NORTH;
        case WEST:
            return secondDir == EAST;
        case EAST:
            return secondDir == WEST;
        default:
            return false;
    }
}

bool returnCanPassThrough(std::pair<int, int> coordinate) {
    switch (expandedGraph[coordinate.first][coordinate.second]) {
        case COLUMN_EXPANSION:
            if (!pathPoints.count({coordinate.first, coordinate.second - 1}) or !getTile({coordinate.first, coordinate.second + 1})) {
                return false;
            }
            return allowVertical(getTile({coordinate.first, coordinate.second - 1}), getTile({coordinate.first, coordinate.second + 1}));
        case ROW_EXPANSION:
            if (!pathPoints.count({coordinate.first - 1, coordinate.second}) or !getTile({coordinate.first + 1, coordinate.second})) {
                return false;
            }
            return allowHorizontal(getTile({coordinate.first - 1, coordinate.second}), getTile({coordinate.first + 1, coordinate.second}));
        default:
            return true;
    }
}

bool allowVertical(char left, char right) {
    return (left == VERTICAL and right == VERTICAL) or (left == NORTH_WEST and right == NORTH_EAST) or (left == SOUTH_WEST and right == SOUTH_EAST);
}

bool allowHorizontal(char top, char bottom) {
    return (top == HORIZONTAL and bottom == HORIZONTAL) or (top == NORTH_EAST and bottom == NORTH_WEST) or (top == SOUTH_EAST and bottom == SOUTH_WEST);
}

void findPath() {
    frontier.clear();
    point north = point(getNewCoordinates(startingPoint, NORTH), NORTH);
    point south = point(getNewCoordinates(startingPoint, SOUTH), SOUTH);
    point east = point(getNewCoordinates(startingPoint, EAST), EAST);
    point west = point(getNewCoordinates(startingPoint, WEST), WEST);
    frontier.push_back(north);
    frontier.push_back(south);
    frontier.push_back(east);
    frontier.push_back(west);
    while (!frontier.empty()) {
        point currPoint = frontier.front();
        frontier.pop_front();
        if (getTile(currPoint.coordinates) == STARTING_POINT)
            return;
        if (getTile(currPoint.coordinates) == GROUND)
            continue;
        if (getTile(currPoint.coordinates) == ROW_EXPANSION or getTile(currPoint.coordinates) == COLUMN_EXPANSION) {
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, currPoint.entranceDirection), currPoint.entranceDirection));
            continue;
        }
        if (pathPoints.count(currPoint.coordinates))
            continue;
        std::pair<direction, direction> possibleDirections = getJoiningDirections(getTile(currPoint.coordinates));
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first) and !isOpposite(currPoint.entranceDirection, possibleDirections.second))
            continue;
        pathPoints[currPoint.coordinates] = 1;
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first))
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, possibleDirections.first), possibleDirections.first));
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.second))
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, possibleDirections.second), possibleDirections.second));
    }
}

/*void findPath() {
    frontier.clear();
    point north = point(getNewCoordinates(startingPoint, NORTH), NORTH);
    point south = point(getNewCoordinates(startingPoint, SOUTH), SOUTH);
    point east = point(getNewCoordinates(startingPoint, EAST), EAST);
    point west = point(getNewCoordinates(startingPoint, WEST), WEST);
    frontier.push_back(north);
    frontier.push_back(south);
    frontier.push_back(east);
    frontier.push_back(west);
    while (!frontier.empty()) {
        point currPoint = frontier.front();
        frontier.pop_front();
        if (getTile(currPoint.coordinates) == STARTING_POINT)
            return;
        if (getTile(currPoint.coordinates) == GROUND)
            continue;
        if (pathPoints.count(currPoint.coordinates))
            continue;
        std::pair<direction, direction> possibleDirections = getJoiningDirections(getTile(currPoint.coordinates));
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first) and !isOpposite(currPoint.entranceDirection, possibleDirections.second))
            continue;
        pathPoints[currPoint.coordinates] = 1;
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first))
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, possibleDirections.first), possibleDirections.first));
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.second))
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, possibleDirections.second), possibleDirections.second));
    }
}*/

int findingIslands() {
    int totalTiles = 0;
    for (std::map<std::pair<int, int>, int>::iterator it = pathPoints.begin(); it != pathPoints.end(); ++it) {
        if (it->first.first > 0)
            totalTiles += numTilesInIsland(getNewCoordinates(it->first, NORTH));
        if (it->first.first < m - 1)
            totalTiles += numTilesInIsland(getNewCoordinates(it->first, SOUTH));
        if (it->first.second > 0)
            totalTiles += numTilesInIsland(getNewCoordinates(it->first, WEST));
        if (it->first.second < n - 1)
            totalTiles += numTilesInIsland(getNewCoordinates(it->first, EAST));
    }
    return totalTiles;
}

int numTilesInIsland(std::pair<int, int> startingPoint) {
    if (pathPoints.count(startingPoint) or exploredIslandPoints.count(startingPoint))
        return 0;
    bool isValidIsland = true;
    std::map<std::pair<int, int>, int> exploredPoints = {};
    std::vector<std::pair<int, int>> points = {};
    frontier.clear();
    frontier.push_back(point(getNewCoordinates(startingPoint, NORTH), NORTH));
    frontier.push_back(point(getNewCoordinates(startingPoint, SOUTH), SOUTH));
    frontier.push_back(point(getNewCoordinates(startingPoint, EAST), EAST));
    frontier.push_back(point(getNewCoordinates(startingPoint, WEST), WEST));
    exploredIslandPoints[startingPoint] = 1;
    points.push_back(startingPoint);
    int numTiles = 1;
    while (!frontier.empty()) {
        point currPoint = frontier.front();
        frontier.pop_back();
        if (exploredIslandPoints.count(currPoint.coordinates))
            continue;
        numTiles += 1;
        exploredIslandPoints[currPoint.coordinates] = 1;
        if (currPoint.coordinates.first == 0 or currPoint.coordinates.first == m - 1)
            isValidIsland = false;
        if (currPoint.coordinates.second == 0 or currPoint.coordinates.second == n - 1)
            isValidIsland = false;
        points.push_back(currPoint.coordinates);
        if (currPoint.coordinates.first > 0)
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, NORTH), NORTH));
        if (currPoint.coordinates.first < m - 1)
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, SOUTH), SOUTH));
        if (currPoint.coordinates.second < n - 1)
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, EAST), EAST));
        if (currPoint.coordinates.second > 0)
            frontier.push_back(point(getNewCoordinates(currPoint.coordinates, WEST), WEST));
    }
    if (!isValidIsland)
        return 0;
    std::cout << points.size() << "\n";
    for (std::pair<int, int> c : points) {
        islandPoints[c] = 1;
    }
    return numTiles;
}

void printPath() {
    std::ofstream writer;
    writer.open("path4.txt");
    if (writer.is_open())
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pathPoints.count({i, j})) {
                    writer << expandedGraph[i][j];
                } else if (islandPoints.count({i, j})) {
                    writer << '!';
                } else if (expandedGraph[i][j] == COLUMN_EXPANSION or expandedGraph[i][j] == ROW_EXPANSION or expandedGraph[i][j] == PADDING) {
                    writer << expandedGraph[i][j];
                } else {
                    writer << '.';
                }
            }
            writer << "\n";
        }
    writer.close();
}

void expandResolution() {
    for (int i = 0; i < graph.size(); i++) {
        expandedGraph.push_back({});
        for (int j = 0; j < graph[0].size(); j++) {
            expandedGraph[i * 2].push_back(graph[i][j]);
            expandedGraph[i * 2].push_back(COLUMN_EXPANSION);
        }
        expandedGraph.push_back({});
        for (int j = 0; j < graph[0].size(); j++) {
            expandedGraph[i * 2 + 1].push_back(ROW_EXPANSION);
            expandedGraph[i * 2 + 1].push_back(PADDING);
        }
    }
    m = expandedGraph.size();
    n = expandedGraph[0].size();
}

void printExpandedGraph() {
    std::ofstream writer;
    writer.open("path3.txt");
    if (writer.is_open())
        for (int i = 0; i < expandedGraph.size(); i++) {
            for (int j = 0; j < expandedGraph[0].size(); j++) {
                writer << expandedGraph[i][j];
            }
            writer << "\n";
        }
    writer.close();
}