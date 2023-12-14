#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define FIXED_STONE '#'
#define ROLLABLE_STONE 'O'
#define EMPTY_SPACE '.'

std::vector<std::string> dish = {};
int numRows;
int numCols;

void processCol(int col, int* finalOutput);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No file name provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    int finalOutput = 0;
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

    for (int col = 0; col < numCols; col++) {
        processCol(col, &finalOutput);
    }

    std::cout << finalOutput << "\n";
}

void processCol(int col, int* finalOutput) {
    int rowCounter = 0;
    for (int row = 0; row < numRows; row++) {
        if (dish[row][col] == ROLLABLE_STONE) {
            *finalOutput += (numRows - rowCounter);
            rowCounter++;
        } else if (dish[row][col] == FIXED_STONE) {
            rowCounter = row + 1;
        }
    }
}