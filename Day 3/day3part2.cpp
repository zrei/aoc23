#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

template <typename T>
void printVec(std::vector<T> const& vec);
int processInput(std::vector<std::string> const& vec, int const m, int const n);
int processGear(int const row, int const col, std::vector<std::string> const& vec, int const m, int const n);
std::pair<int, int> findNumber(int const row, int col, int* number, int n, std::string line);
bool isNumber(char const c);
bool isSymbol(char const c);
bool foundRange(std::map<int, std::vector<std::pair<int, int>>>& ranges, int const row, int const col);

// idea: for each gear, check the six possible directions
// for each direction, find the full number and then note the index it is under
// then when finding numbers again for the other six check against these numbers. if inside, ignore

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }
    std::ifstream myFile;
    myFile.open(argv[1]);
    std::vector<std::string> vec = {};
    std::string s;
    int m;
    int n = -1;
    if (myFile.is_open())
        while (myFile.good()) {
            std::getline(myFile, s);
            vec.push_back(s);
            if (myFile.eof())
                break;
        }
    m = vec.size();
    n = vec[0].size();
    std::cout << m << ", " << n << "\n";
    
    int finalOutput = processInput(vec, m, n);
    std::cout << finalOutput << "\n";
}

template <typename T>
void printVec(std::vector<T> const& vec) {
    std::cout << "Printing vector: ";
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

std::pair<int, int> findNumber(int const row, int col, int* number, int n, std::string line) {
    int lp = col;
    int rp = col;
    bool finished = false;
    while (!finished) {
        bool leftFinished = false;
        if (lp == 0)
            leftFinished = true;
        else if (!isNumber(line[lp - 1]))
            leftFinished = true;
        else
            lp--;
        
        bool rightFinished = false;
        if (rp == n - 1)
            rightFinished = true;
        else if (!isNumber(line[rp + 1]))
            rightFinished = true;
        else
            rp++;
        finished = leftFinished and rightFinished;
    }
    int temp = 0;
    for (int i = lp; i <= rp; i++) {
        temp *= 10;
        temp += line[i] - '0';
    }
    *number *= temp;
    return {lp, rp};
}

int processInput(std::vector<std::string> const& vec, int const m, int const n) {
    int out = 0;
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            if (isSymbol(vec[row][col]))
                out += processGear(row, col, vec, m, n);
            std::cout << "Completed: " << row << ", " << col << "\n";
        }
    }
    return out;
}

int processGear(int const row, int const col, std::vector<std::string> const& vec, int const m, int const n) {
    std::map<int, std::vector<std::pair<int, int>>> ranges = {};
    int initialNumber = 1;
    int numNumbers = 0;
    for (int i = -1; i < 2; i++) {
        if (row + i < 0 or row + i >= m)
            continue;
        for (int j = -1; j < 2; j++) {
            if (col + i < 0 or col + i >= n)
                continue;
            if (isNumber(vec[row + i][col + j]) and !foundRange(ranges, row + i, col + j)) {
                std::pair<int, int> p = findNumber(row + i, col + j, &initialNumber, n, vec[row + i]);
                numNumbers++;
                if (!ranges.count(row + i))
                    ranges[row + i] = {};
                ranges[row + i].push_back(p);
            }
        }
    }
    if (numNumbers == 2)
        return initialNumber;
    else
        return 0;
}

bool foundRange(std::map<int, std::vector<std::pair<int, int>>>& ranges, int const row, int const col) {
    if (ranges.count(row)) {
        for (std::pair<int, int> p : ranges[row]) {
            if (col >= p.first and col <= p.second)
                return true;
        }
    }
    return false;
}

bool isNumber(char const c) {
    return c >= 48 and c <= 57;
}

bool isSymbol(char const c) {
    return c == '*';
}