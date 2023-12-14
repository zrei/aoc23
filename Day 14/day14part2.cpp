#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

#define FIXED_STONE '#'
#define ROLLABLE_STONE 'O'
#define EMPTY_SPACE '.'

std::vector<std::string> dish = {};
int numRows;
int numCols;
std::map<std::string, int> experiencedDishes = {};
int loopIndex = 0;
int loopLength;

bool oneCycle();
void rollWest(int row);
void rollEast(int row);
void rollSouth(int col);
void rollNorth(int col);
std::string constructString();
void printDish();
int countStones(int numTimesToRepeat);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No file name provided\n";
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
            dish.push_back(s);
        }
    }
    numRows = dish.size();
    numCols = dish[0].size();

    for (int i = 0; i < 1000000000; i++) {
        if (oneCycle())
            break;
    }

    int numTimesToRepeat = ((1000000000 - loopIndex) % loopLength);
    std::cout << countStones(numTimesToRepeat) << "\n";
}

int countStones(int numTimesToRepeat) {
    for (int i = 0; i < numTimesToRepeat; i++) {
        for (int col = 0; col < numCols; col++)
            rollNorth(col);
        for (int row = 0; row < numRows; row++)
            rollWest(row);
        for (int col = 0; col < numCols; col++)
            rollSouth(col);
        for (int row = 0; row < numRows; row++)
            rollEast(row);
    }
    int finalAmount = 0;
    for (int row = 0; row < numRows; row++) {
        for (char c : dish[row]) {
            if (c == ROLLABLE_STONE)
                finalAmount += numRows - row;
        }
    }
    return finalAmount;
}

void rollNorth(int col) {
    int rowCounter = 0;
    for (int row = 0; row < numRows; row++) {
        if (dish[row][col] == ROLLABLE_STONE) {
            if (row != rowCounter) {
                dish[rowCounter][col] = ROLLABLE_STONE;
                dish[row][col] = EMPTY_SPACE;
            }
            rowCounter++;
        } else if (dish[row][col] == FIXED_STONE) {
            rowCounter = row + 1;
        }
    }
}

void rollSouth(int col) {
    int rowCounter = numRows - 1;
    for (int row = numRows - 1; row >= 0; row--) {
        if (dish[row][col] == ROLLABLE_STONE) {
            if (row != rowCounter) {
                dish[rowCounter][col] = ROLLABLE_STONE;
                dish[row][col] = EMPTY_SPACE;
            }
            rowCounter--;
        } else if (dish[row][col] == FIXED_STONE) {
            rowCounter = row - 1;
        }
    }
}

void rollEast(int row) {
    int colCounter = numCols - 1;
    for (int col = numCols - 1; col >= 0; col--) {
        if (dish[row][col] == ROLLABLE_STONE) {
            if (col != colCounter) {
                dish[row][colCounter] = ROLLABLE_STONE;
                dish[row][col] = EMPTY_SPACE;
            }
            colCounter--;
        } else if (dish[row][col] == FIXED_STONE) {
            colCounter = col - 1;
        }
    }
}

void rollWest(int row) {
    int colCounter = 0;
    for (int col = 0; col < numCols; col++) {
        if (dish[row][col] == ROLLABLE_STONE) {
            if (col != colCounter) {
                dish[row][colCounter] = ROLLABLE_STONE;
                dish[row][col] = EMPTY_SPACE;
            }
            colCounter++;
        } else if (dish[row][col] == FIXED_STONE) {
            colCounter = col + 1;
        }
    }
}

bool oneCycle() {
    printDish();
    std::string curr = constructString();
    if (experiencedDishes.count(curr)) {
        loopLength = loopIndex - experiencedDishes[curr];
        loopIndex = experiencedDishes[curr];
        return true;
    }
    experiencedDishes[curr] = loopIndex;
    for (int col = 0; col < numCols; col++)
        rollNorth(col);
    for (int row = 0; row < numRows; row++)
        rollWest(row);
    for (int col = 0; col < numCols; col++)
        rollSouth(col);
    for (int row = 0; row < numRows; row++)
        rollEast(row);
    loopIndex++;
    return false;
}

std::string constructString() {
    std::string output = "";
    for (std::string s : dish) 
        output += s;
    return output;
}

void printDish() {
    for (std::string s : dish) {
        std::cout << s << "\n";
    }
    std::cout << "\n";
}