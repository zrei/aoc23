#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define SEPARATOR ','
#define DASH '-'
#define EQUALS '='
#define NUM_BOXES 256

std::map<int, std::vector<std::pair<std::string, int>>> boxes = {};

void processLine(std::string const& s);
void insertLens(int focalLens, std::string label, int boxNum);
void removeLens(int boxNum, std::string label);
long long calculateFocusingPower();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    getline(myFile, s);
    processLine(s);
    std::cout << calculateFocusingPower() << "\n";
}

void processLine(std::string const& s) {
    int lp = 0;
    int rp = 0;
    int boxNum = 0;
    while (rp < s.size()) {
        if (s[rp] == EQUALS) {
            rp++;
            int focalLens = s[rp] - '0';
            insertLens(focalLens, s.substr(lp, rp - lp - 1), boxNum);
            boxNum = 0;
            rp++;
            lp = rp + 1;
        } else if (s[rp] == DASH) {
            removeLens(boxNum, s.substr(lp, rp - lp));
            rp++;
            lp = rp + 1;
            boxNum = 0;
        } else {
            boxNum += s[rp];
            boxNum *= 17;
            boxNum = boxNum % 256;
        }
        rp++;
    }
}

void insertLens(int focalLens, std::string label, int boxNum) {
    if (!boxes.count(boxNum)) {
        boxes[boxNum] = {};
    }
    bool labelFound = false;
    std::vector<std::pair<std::string, int>> temp = {};
    for (std::pair<std::string, int> p : boxes[boxNum]) {
        if (p.first == label) {
            temp.push_back({label, focalLens});
            labelFound = true;
        } else {
            temp.push_back(p);
        }
    }
    if (!labelFound) {
        temp.push_back({label, focalLens});
    }
    boxes[boxNum] = temp;
}

void removeLens(int boxNum, std::string label) {
    if (!boxes.count(boxNum))
        return;
    std::vector<std::pair<std::string, int>> temp = {};
    for (std::pair<std::string, int> p : boxes[boxNum]) {
        if (p.first != label)
            temp.push_back(p);
    }
    boxes[boxNum] = temp;
}

long long calculateFocusingPower() {
    long long focusingPower = 0;
    for (int i = 0; i < NUM_BOXES; i++) {
        if (boxes.count(i))
            for (int j = 0; j < boxes[i].size(); j++)
                focusingPower += (i + 1) * (j + 1) * (boxes[i][j].second);
    }
    return focusingPower;
}