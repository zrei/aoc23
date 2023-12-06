#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

std::vector<int> times = {};
std::vector<int> distances = {};

bool isNumber(char c);
int processRace(int time, int distance);
int discriminant(int a, int b, int c);
bool hasSolution(int a, int b, int c);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    int i = -1;
    if (myFile.is_open()) {
        while (myFile.good()) {
            myFile >> s;
            std::cout << s << "\n";
            if (!isNumber(s[0])) {
                i++;
            } else {
                switch (i) {
                    case 0:
                        times.push_back(std::stoi(s));
                        break;
                    case 1:
                        std::cout << "Distance push: " << s << "\n";
                        distances.push_back(std::stoi(s));
                        break;
                }
            }
            if (myFile.eof()) {
                break;
            }
        }
    }
    int numRaces = times.size();
    int finalOutput = 0;
    for (int i = 0; i < numRaces; i++) {
        int processedNumber = processRace(times[i], distances[i]);
        std::cout << "Processed number: " << processedNumber << "\n";
        if (processedNumber > 0) {
            if (finalOutput == 0)
                finalOutput = processedNumber;
            else
                finalOutput *= processedNumber;
        }
    }
    std::cout << finalOutput << "\n";
}

bool isNumber(char c) {
    return c >= 48 and c <= 57;
}

int processRace(int time, int distance) {
    std::cout << "Discriminant: " << discriminant(1, -time, distance) << "\n";
    if (hasSolution(1, -time, distance)) {
        float solution1 = (-(-time) + sqrt(discriminant(1, -time, distance))) / 2.0;
        float solution2 = (-(-time) - sqrt(discriminant(1, -time, distance))) / 2.0;
        std::cout << "Solutions: " << solution1 << ", " << solution2 << "\n";
        if (solution2 > solution1) {
            return std::floor(solution2) - std::max(int(std::ceil(solution1)), 0) + 1;
        } else {
            return std::floor(solution1) - std::max(int(std::ceil(solution2)), 0) + 1;
        }
    } else {
        return 0;
    }
}

int discriminant(int a, int b, int c) {
    return b * b - 4 * a * c;
}

bool hasSolution(int a, int b, int c) {
    return discriminant(a, b, c) >= 0;
}