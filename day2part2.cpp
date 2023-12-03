#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

void splitString(std::string line, std::vector<std::string>& splitString);

template <typename T>
void printVector(std::vector<T> vec);

int findPower(std::vector<std::string> const& splitStrings);

void findMinimumInSet(int* index, int* red, int* green, int* blue, std::vector<std::string> const& splitStrings);

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
        finalOutput += findPower(splitStrings);
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

template <typename T>
void printVector(std::vector<T> vec) {
    std::cout << "Printing vector: ";
    for (T i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

int findPower(std::vector<std::string> const& splitStrings) {
    int index = 2;
    int red = 0;
    int green = 0;
    int blue = 0;
    while (index < splitStrings.size()) {
        findMinimumInSet(&index, &red, &green, &blue, splitStrings);
    }
    return red * green * blue;
}


void findMinimumInSet(int* index, int* red, int* green, int* blue, std::vector<std::string> const& splitStrings) {
    bool finished = false;
    while (!finished) {
        char startingLetter = splitStrings[*index + 1][0];
        switch (startingLetter)
        {
        case 'r':
            *red = std::max(*red, std::stoi(splitStrings[*index]));
            break;
        case 'g':
            *green = std::max(*green, std::stoi(splitStrings[*index]));
            break;
        case 'b':
            *blue = std::max(*blue, std::stoi(splitStrings[*index]));
        default:
            break;
        }
        char endingLetter = splitStrings[*index + 1][splitStrings[*index + 1].size() - 1];
        if (endingLetter != ',')
            finished = true; 
        (*index) += 2;
    }
}