#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAIN_SEPARATOR '@'
#define X 0
#define Y 1
#define Z 2

#define MIN_RANGE 200000000000000
#define MAX_RANGE 400000000000000

struct hailstone {
    long long xPos;
    long long yPos;
    long long zPos;
    long long xVel;
    long long yVel;
    long long zVel;

    hailstone(long long xPos, long long yPos, long long zPos, long long xVel, long long yVel, long long zVel) {
        this->xPos = xPos;
        this->yPos = yPos;
        this->zPos = zPos;
        this->xVel = xVel;
        this->yVel = yVel;
        this->zVel = zVel;
    }
};

std::vector<hailstone> hailstones = {};

void processLine(std::string s);
std::vector<long long> processValues(std::string s);
bool checkIfParallel(long long xVel1, long long xVel2, long long yVel1, long long yVel2);
double calculateGradient(long long xVel, long long yVel);
std::pair<double, double> calculateTime(long long xPos1, long long xVel1, long long xPos2, long long xVel2, long long yPos1, long long yVel1, long long yPos2, long long yVel2);
void findPairs(long long* numPairs);

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
            processLine(s);
        }
    }

    long long numPairs = 0;
    findPairs(&numPairs);

    std::cout << "Final number of pairs: " << numPairs << "\n";
}

void processLine(std::string s) {
    int sepPos = s.find(MAIN_SEPARATOR);
    std::string positions = s.substr(0, sepPos - 1);
    std::string velocities = s.substr(sepPos + 2);
    std::vector<long long> pos = processValues(positions);
    std::vector<long long> vel = processValues(velocities);
    hailstones.push_back(hailstone(pos[X], pos[Y], pos[Z], vel[X], vel[Y], vel[Z]));
}

std::vector<long long> processValues(std::string s) {
    int rp = 0;
    std::vector<long long> values = {};
    long currValue = 0;
    bool isNegative = false;
    while (rp < s.size()) {
        if (s[rp] == ',') {
            values.push_back(isNegative ? -currValue : currValue);
            rp += 2;
            currValue = 0;
            isNegative = false;
        } else if (s[rp] == '-') {
            isNegative = true;
            rp++;
        } else if (s[rp] == ' ') {
            rp++;
        }
        else {
            currValue *= 10;
            currValue += s[rp] - '0';
            rp++;
        }
    }
    values.push_back(isNegative ? -currValue : currValue);
    return values;
}

bool checkIfParallel(long long xVel1, long long xVel2, long long yVel1, long long yVel2) {
    return calculateGradient(xVel1, yVel1) == calculateGradient(xVel2, yVel2);
}

double calculateGradient(long long xVel, long long yVel) {
    return yVel / (double) xVel;
}

std::pair<double, double> calculateTime(long long xPos1, long long xVel1, long long xPos2, long long xVel2, long long yPos1, long long yVel1, long long yPos2, long long yVel2) {
    double t2 = (yPos2 - yPos1 + xPos1 * yVel1 / (double) xVel1 - yVel1 * xPos2 / (double) xVel1) / (yVel1 * xVel2 / (double) xVel1 - yVel2);
    double t1 = (xPos2 + xVel2 * t2 - xPos1) / (double) xVel1;
    std::cout << "Times: " << t1 << ", " << t2 << "\n";
    return {t1, t2};
}

void findPairs(long long* numPairs) {
    for (int i = 0; i < hailstones.size() - 1; i++) {
        for (int j = i + 1; j < hailstones.size(); j++) {
            if (checkIfParallel(hailstones[i].xVel, hailstones[j].xVel, hailstones[i].yVel, hailstones[j].yVel)) {
                std::cout << "Pairs " << i << " and " << j << " are parallel\n";
                continue;
            }
            std::pair<double, double> time = calculateTime(hailstones[i].xPos, hailstones[i].xVel, hailstones[j].xPos, hailstones[j].xVel, hailstones[i].yPos, hailstones[i].yVel, hailstones[j].yPos, hailstones[j].yVel);
            if (time.first <= 0 or time.second <= 0) {
                std::cout << "Pairs " << i << " and " << j << " intersect in the past for at least one of them\n";
                continue;
            }
            double finalX = hailstones[i].xPos + time.first * hailstones[i].xVel;
            double finalY = hailstones[i].yPos + time.first * hailstones[i].yVel;
            if (finalX < MIN_RANGE or finalX > MAX_RANGE or finalY < MIN_RANGE or finalY > MAX_RANGE) {
                std::cout << "Pairs " << i << " and " << j << " intersect outside the range\n";
                continue;
            }
            (*numPairs)++;
            std::cout << "Pairs " << i << " and " << j << " intersect at " << finalX << ", " << finalY << "\n";
        }
    }
}