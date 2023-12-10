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
void replaceStartingPoint();
int findEnclosed();

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
    //expandResolution();
    m = graph.size();
    n = graph[0].size();
    findStartingPoint();
    findPath();
    replaceStartingPoint();
    printPath();
    std::cout << findEnclosed() << "\n";
    //findStartingPoint();
    //findPath();
    //printGraph();
    //std::cout << findingIslands() << "\n";
}

void fillGraph(std::string s) {
    graph.push_back(s);
}

void replaceStartingPoint() {
    std::pair<int, int> northCoordinates = getNewCoordinates(startingPoint, NORTH);
    std::pair<int, int> southCoordinates = getNewCoordinates(startingPoint, SOUTH);
    std::pair<int, int> westCoordinates = getNewCoordinates(startingPoint, WEST);
    std::pair<int, int> eastCoordinates = getNewCoordinates(startingPoint, EAST);
    bool north = pathPoints.count(getNewCoordinates(startingPoint, NORTH)) and (getTile(northCoordinates) == VERTICAL or getTile(northCoordinates) == SOUTH_EAST or getTile(northCoordinates) == SOUTH_WEST);
    bool south = pathPoints.count(getNewCoordinates(startingPoint, SOUTH)) and (getTile(southCoordinates) == VERTICAL or getTile(southCoordinates) == NORTH_EAST or getTile(southCoordinates) == NORTH_WEST);
    bool east = pathPoints.count(getNewCoordinates(startingPoint, EAST)) and (getTile(eastCoordinates) == HORIZONTAL or getTile(eastCoordinates) == SOUTH_WEST or getTile(eastCoordinates) == NORTH_WEST);
    bool west = pathPoints.count(getNewCoordinates(startingPoint, WEST)) and (getTile(westCoordinates) == HORIZONTAL or getTile(westCoordinates) == SOUTH_EAST or getTile(westCoordinates) == NORTH_EAST);
    if (north and south) {
        graph[startingPoint.first][startingPoint.second] = VERTICAL;
    } else if (east and west) {
        graph[startingPoint.first][startingPoint.second] = HORIZONTAL;
    } else if (north and east) {
        graph[startingPoint.first][startingPoint.second] = NORTH_EAST;
    } else if (north and west) {
        graph[startingPoint.first][startingPoint.second] = NORTH_WEST;
    } else if (south and east) {
        graph[startingPoint.first][startingPoint.second] = SOUTH_EAST;
    } else if (south and west) {
        graph[startingPoint.first][startingPoint.second] = SOUTH_WEST;
    }
}

void findStartingPoint() {
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (graph[row][col] == STARTING_POINT) {
                startingPoint = {row, col};
                return;
            }
        }
    }
}

char getTile(std::pair<int, int> coordinates) {
    return graph[coordinates.first][coordinates.second];
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


void findPath() {
    frontier.clear();
    point north = point(getNewCoordinates(startingPoint, NORTH), NORTH);
    point south = point(getNewCoordinates(startingPoint, SOUTH), SOUTH);
    point east = point(getNewCoordinates(startingPoint, EAST), EAST);
    point west = point(getNewCoordinates(startingPoint, WEST), WEST);
    pathPoints[startingPoint] = 1;
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
}

bool upFacing(char c) {
    return c == VERTICAL or c == SOUTH_WEST or c == SOUTH_EAST;
}

int findEnclosed() {
    bool insideLoop = false;
    int numEnclosed = 0;
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (insideLoop and !pathPoints.count({row, col}))
                numEnclosed++;
            if (pathPoints.count({row, col}) and upFacing(getTile({row, col}))) {
                insideLoop = !insideLoop;
            }
        }
    }
    return numEnclosed;
}

void printPath() {
    std::ofstream writer;
    writer.open("path5.txt");
    if (writer.is_open())
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pathPoints.count({i, j})) {
                    writer << graph[i][j];
                } else {
                    writer << '.';
                }
            }
            writer << "\n";
        }
    writer.close();
}