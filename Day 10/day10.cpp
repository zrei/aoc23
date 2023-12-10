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
std::deque<std::pair<point, int>> frontier = {};

void fillGraph(std::string s);
void findStartingPoint();
bool isOpposite(direction firstDir, direction secondDir);
char getTile(std::pair<int, int> coordinates);
std::pair<direction, direction> getJoiningDirections(char c);
std::pair<int, int> getNewCoordinates(std::pair<int, int> oldCoordinates, direction dir);
bool isOpposite(direction firstDir, direction secondDir);
int bfs();

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
    findStartingPoint();
    std::cout << bfs() << "\n";
}

void fillGraph(std::string s) {
    graph.push_back(s);
}

void findStartingPoint() {
    for (int row = 0; row < graph.size(); row++) {
        for (int col = 0; col < graph[row].size(); col++) {
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

int bfs() {
    point north = point(getNewCoordinates(startingPoint, NORTH), NORTH);
    point south = point(getNewCoordinates(startingPoint, SOUTH), SOUTH);
    point east = point(getNewCoordinates(startingPoint, EAST), EAST);
    point west = point(getNewCoordinates(startingPoint, WEST), WEST);
    frontier.push_back({north, 1});
    frontier.push_back({south, 1});
    frontier.push_back({east, 1});
    frontier.push_back({west, 1});
    while (!frontier.empty()) {
        std::pair<point, int> temp = frontier.front();
        frontier.pop_front();
        point currPoint = temp.first;
        int length = temp.second;
        if (getTile(currPoint.coordinates) == STARTING_POINT)
            return length / 2;
        if (getTile(currPoint.coordinates) == GROUND)
            continue;
        std::pair<direction, direction> possibleDirections = getJoiningDirections(getTile(currPoint.coordinates));
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first) and !isOpposite(currPoint.entranceDirection, possibleDirections.second))
            continue;
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.first))
            frontier.push_back({point(getNewCoordinates(currPoint.coordinates, possibleDirections.first), possibleDirections.first), length + 1});
        if (!isOpposite(currPoint.entranceDirection, possibleDirections.second))
            frontier.push_back({point(getNewCoordinates(currPoint.coordinates, possibleDirections.second), possibleDirections.second), length + 1});
    }
    return 0;
}