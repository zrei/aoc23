#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

int processLine(std::string s);
std::pair<std::string, std::string> splitLine(std::string const& s);
void findWinningNumbers(std::map<int, int>& winningNumbers, std::string const& s);
int findCardValue(std::map<int, int>& winningNumbers, std::string const& s);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided!\n";
        return 0;
    }
    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);
    int finalOutput = 0;
    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, s);
            if (myFile.eof()) 
                break;
            finalOutput += processLine(s);
        }
    }
    std::cout << finalOutput << "\n";
}

int processLine(std::string s) {
    std::pair<std::string, std::string> split = splitLine(s);
    std::map<int, int> winningNumbers = {};
    findWinningNumbers(winningNumbers, split.second);
    return findCardValue(winningNumbers, split.first);
}

std::pair<std::string, std::string> splitLine(std::string const& s) {
    int sepIndex = s.find('|');
    return {s.substr(0, sepIndex), s.substr(sepIndex + 2, s.size() - (sepIndex + 2))};
}

void findWinningNumbers(std::map<int, int>& winningNumbers, std::string const& s) {
    int lp = 0;
    int rp = 0;
    bool isNumber = false;
    while (rp < s.size()) {
        if (s[rp] == ' ') {
            if (isNumber)
                winningNumbers[std::stoi(s.substr(lp, rp - lp + 1))] = 1;
            lp = rp + 1;
            isNumber = false;
        }
        else
            isNumber = true;
        rp++;
    }
    winningNumbers[std::stoi(s.substr(lp, rp - lp + 1))] = 1;
}

int findCardValue(std::map<int, int>& winningNumbers, std::string const& s) {
    int cardValue = 0;
    int lp = s.find(':') + 1;
    int rp = lp;
    bool isNumber = false;
    while (rp < s.size()) {
        if (s[rp] == ' ') {
            if (isNumber and winningNumbers.count(std::stoi(s.substr(lp, rp - lp + 1)))) {
                if (cardValue == 0)
                    cardValue = 1;
                else
                    cardValue <<= 1;
            }
            isNumber = false;
            lp = rp + 1;
        }
        else
            isNumber = true;
        rp++;
    }
    return cardValue;
}