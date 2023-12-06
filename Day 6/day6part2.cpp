#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

long long parsedTime = 0;
long long parsedDistance = 0;

bool isNumber(char c);
long long processRace(long long time, long long distance);
long long discriminant(long long a, long long b, long long c);
bool hasSolution(long long a, long long b, long long c);

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
                int parsed = std::stoi(s);
                switch (i) {
                    case 0:
                        parsedTime *= pow(10, s.size());
                        parsedTime += parsed;
                        break;
                    case 1:
                        parsedDistance *= pow(10, s.size());
                        parsedDistance += parsed;
                        break;
                }
            }
            if (myFile.eof()) {
                break;
            }
        }
    }
    std::cout << "Final time: " << parsedTime << "\n";
    std::cout << "Final distance: " << parsedDistance << "\n";
    long long finalOutput = processRace(parsedTime, parsedDistance);
    std::cout << finalOutput << "\n";
}

bool isNumber(char c) {
    return c >= 48 and c <= 57;
}

long long processRace(long long time, long long distance) {
    std::cout << "Discriminant: " << discriminant(1, -time, distance) << "\n";
    if (hasSolution(1, -time, distance)) {
        double solution1 = (-(-time) + sqrt(discriminant(1, -time, distance))) / 2.0;
        double solution2 = (-(-time) - sqrt(discriminant(1, -time, distance))) / 2.0;
        std::cout << "Solutions: " << solution1 << ", " << solution2 << "\n";
        if (solution2 > solution1) {
            long long roundedSol = std::ceil(solution1);
            return std::floor(solution2) - std::max(roundedSol, (long long) 0) + 1;
        } else {
            long long roundedSol = std::ceil(solution2);
            return std::floor(solution1) - std::max(roundedSol, (long long) 0) + 1;
        }
    } else {
        return 0;
    }
}

long long discriminant(long long a, long long b, long long c) {
    return b * b - 4 * a * c;
}

bool hasSolution(long long a, long long b, long long c) {
    return discriminant(a, b, c) >= 0;
}