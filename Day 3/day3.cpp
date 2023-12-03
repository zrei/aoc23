#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

template <typename T>
void printVec(std::vector<T> const& vec);
void process(int* finalOutput, std::vector<std::string> const& vec, int const m, int const n);
bool isNumber(char const c);
bool isSymbol(char const c);

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
    
    int finalOutput = 0;
    process(&finalOutput, vec, m, n);
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

void process(int* finalOutput, std::vector<std::string> const& vec, int const m, int const n) {
    int rp = 0;
    int currNum = 0;
    bool toInclude = false;
    for (int row = 0; row < m; row++) {
        rp = 0;
        currNum = 0;
        toInclude = false;
        while (rp < n) {
            if (!isNumber(vec[row][rp])) {
                if (toInclude)
                    *finalOutput += currNum;
                currNum = 0;
                toInclude = false;
            } else {
                currNum *= 10;
                currNum += vec[row][rp] - '0';
                if (rp < n - 1) 
                    toInclude = toInclude || isSymbol(vec[row][rp + 1]);
                
                if (rp > 0) 
                    toInclude = toInclude || isSymbol(vec[row][rp - 1]);
                
                if (row < m - 1)
                    toInclude = toInclude || isSymbol(vec[row + 1][rp]);
                
                if (row < m - 1 and rp > 0)
                    toInclude = toInclude || isSymbol(vec[row + 1][rp - 1]);
                
                if (row < m - 1 and rp < n - 1) 
                    toInclude = toInclude || isSymbol(vec[row + 1][rp + 1]);
                
                if (row > 0) 
                    toInclude = toInclude || isSymbol(vec[row - 1][rp]);
                
                if (row > 0 and rp > 0) 
                    toInclude = toInclude || isSymbol(vec[row - 1][rp - 1]);
                
                if (row > 0 and rp < n - 1) 
                    toInclude = toInclude || isSymbol(vec[row - 1][rp + 1]);
                
            }
            rp++;
        }
        if (toInclude)
            *finalOutput += currNum;
    }
}

bool isNumber(char const c) {
    return c >= 48 and c <= 57;
}

bool isSymbol(char const c) {
    char bannedSymbol = '.';
    return !isNumber(c) and c != bannedSymbol;
}