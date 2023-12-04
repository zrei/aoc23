#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

void processLine(std::string s, std::vector<int>* numCards, int index);
std::pair<std::string, std::string> splitLine(std::string const& s);
void findWinningNumbers(std::map<int, int>& winningNumbers, std::string const& s);
void evaluateCard(std::map<int, int>& winningNumbers, std::string const& s, std::vector<int>* cardNums, int const& cardIndex);
int sumCards(std::vector<int> const& numCards);

template <typename T>
void printVec(std::vector<T> vec);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided!\n";
        return 0;
    }
    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);
    std::vector<int> numCards = {};
    std::vector<std::string> cards = {};
    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, s);
            if (myFile.eof()) 
                break;
            cards.push_back(s);
            numCards.push_back(0);
        }
    }
    int index = 0;
    for (std::string s : cards) {
        processLine(s, &numCards, index);
        index++;
    }
    int finalOutput = sumCards(numCards);
    std::cout << finalOutput << "\n";
}

void processLine(std::string s, std::vector<int>* numCards, int index) {
    std::pair<std::string, std::string> split = splitLine(s);
    std::map<int, int> winningNumbers = {};
    findWinningNumbers(winningNumbers, split.second);
    (*numCards)[index] += 1;
    evaluateCard(winningNumbers, split.first, numCards, index);
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

void evaluateCard(std::map<int, int>& winningNumbers, std::string const& s, std::vector<int> *cardNums, int const& cardIndex) {
    int cardValue = 0;
    int lp = s.find(':') + 1;
    int rp = lp;
    bool isNumber = false;
    int numMatches = 0;
    while (rp < s.size()) {
        if (s[rp] == ' ') {
            if (isNumber and winningNumbers.count(std::stoi(s.substr(lp, rp - lp + 1)))) {
                    numMatches++;
            }
            isNumber = false;
            lp = rp + 1;
        }
        else
            isNumber = true;
        rp++;
    }
    for (int i = 1; i < numMatches + 1; i++)
        (*cardNums)[cardIndex + i] += (*cardNums)[cardIndex];
    
    std::cout << "Finished evaluating card " << cardIndex << "\n";
    std::cout << "Final number for this card: " << (*cardNums)[cardIndex] << "\n";
}

int sumCards(std::vector<int> const& numCards) {
    int sum = 0;
    for (int i : numCards)
        sum += i;
    return sum;
}

template <typename T>
void printVec(std::vector<T> vec) {
    std::cout << "Printing vec: \n";
    for (T i : vec)
        std::cout << i << ", ";
    std::cout << "\n";
}