#include <iostream>
#include <fstream>
#include <vector>
#include <map>

// indicate the selected i as the last row / col in the inner side of the reflection

int processPattern(std::vector<std::string>& pattern);
bool checkHorizontal(std::vector<std::string> const& pattern, int row);
bool checkTwoRows(std::string const& s1, std::string const& s2);
bool checkVertical(std::vector<std::string> const& pattern, int col);
bool checkTwoCols(std::vector<std::string> const& pattern, int col1, int col2);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);
    std::vector<std::string> pattern = {};
    int finalOutput = 0;
    if (myFile.is_open()) {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            while (!s.empty()) {
                pattern.push_back(s);
                getline(myFile, s);
            }
            finalOutput += processPattern(pattern);
            pattern.clear();
        }
    }
    std::cout << finalOutput << "\n";
}

int processPattern(std::vector<std::string>& pattern) {
    for (int i = 0; i < pattern.size() - 1; i++) {
        if (checkHorizontal(pattern, i)) {
            return 100 * (i + 1);
        }
    }
    for (int i = 0; i < pattern[0].size() - 1; i++) {
        if (checkVertical(pattern, i)) {
            return i + 1;
        }
    }
    return 0;
}

bool checkHorizontal(std::vector<std::string> const& pattern, int row) {
    int numTopRows = row + 1;
    int numBottomRows = pattern.size() - row - 1;
    int rows = std::min(numTopRows, numBottomRows);
    for (int i = 0; i < rows; i++) {
        if (!checkTwoRows(pattern[row - i], pattern[row + 1 + i]))
            return false;
    }
    return true;
}

bool checkTwoRows(std::string const& s1, std::string const& s2) {
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i])
            return false;
    }
    return true;
} 

bool checkVertical(std::vector<std::string> const& pattern, int col) {
    int numLeftCols = col + 1;
    int numRightCols = pattern[0].size() - col - 1;
    int cols = std::min(numLeftCols, numRightCols);
    for (int i = 0; i < cols; i++) {
        if (!checkTwoCols(pattern, col - i, col + 1 + i))
            return false;
    }
    return true;
}

bool checkTwoCols(std::vector<std::string> const& pattern, int col1, int col2) {
    for (int i = 0; i < pattern.size(); i++) {
        if (pattern[i][col1] != pattern[i][col2]) {
            return false;
        }
    }
    return true;
}