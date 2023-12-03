#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

void splitString(std::string line, std::vector<std::string>& splitString);

template <typename T>
void printVector(std::vector<T> vec);

int getGameId(std::vector<std::string>& splitStrings);

bool checkOneSet(int* index, int red, int green, int blue, std::vector<std::string> const& splitStrings);

bool checkValidity(int red, int green, int blue, std::vector<std::string> const& splitStrings);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }
    int red = 12;
    int green = 13;
    int blue = 14;

    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);
    int finalOutput = 0;

    if (myFile.is_open()) {
    while (myFile.good()) {
        std::getline(myFile, s);
        if (myFile.eof())
            break;
        std::vector<std::string> splitStrings = {};
        splitString(s, splitStrings);
        //std::cout << "Line: " << s << "\n";
        //std::cout << "Split line: "; 
        //printVector(splitStrings);
        int gameId = getGameId(splitStrings);
        if (checkValidity(red, green, blue, splitStrings)) {
            finalOutput += gameId;
        }
    }

    std::cout << finalOutput << "\n";
  }
}

void splitString(std::string line, std::vector<std::string>& splitStrings) {
    int pos = 0;
    int prevPos = -1;
    while (true) {
        pos = line.find(' ', prevPos + 1);
        if (pos == -1)
            break;
        splitStrings.push_back(line.substr(prevPos + 1, pos - prevPos - 1));
        prevPos = pos;
    } 
    splitStrings.push_back(line.substr(prevPos + 1));
}

int getGameId(std::vector<std::string>& splitStrings) {
    std::string numberString = splitStrings[1].substr(0, splitStrings[1].size() - 1);
    std::cout << numberString << "\n";
    return std::stoi(numberString);
}

template <typename T>
void printVector(std::vector<T> vec) {
    std::cout << "Printing vector: ";
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

bool checkValidity(int red, int green, int blue, std::vector<std::string> const& splitStrings) {
    int index = 2;
    while (index < splitStrings.size()) {
        bool valid = checkOneSet(&index, red, green, blue, splitStrings);
        if (!valid)
            return false;
    }
    return true;
}


bool checkOneSet(int* index, int red, int green, int blue, std::vector<std::string> const& splitStrings) {
    bool finished = false;
    while (!finished) {
        bool valid = true;
        char startingLetter = splitStrings[*index + 1][0];
        switch (startingLetter)
        {
        case 'r':
            valid = valid && std::stoi(splitStrings[*index]) <= red;
            break;
        case 'g':
            valid = valid && std::stoi(splitStrings[*index]) <= green;
            break;
        case 'b':
            valid = valid && std::stoi(splitStrings[*index]) <= blue;
        default:
            break;
        }
        if (!valid)
            return false;
        char endingLetter = splitStrings[*index + 1][splitStrings[*index + 1].size() - 1];
        if (endingLetter != ',')
            finished = true; 
        (*index) += 2;
    }
    return true;
}