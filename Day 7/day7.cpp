#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

enum handType {
    FIVE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    THREE_OF_A_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD
};

std::map<char, int> letterStrength = {
    {'A', 4},
    {'K', 3},
    {'Q', 2},
    {'J', 1},
    {'T', 0}
};

void insertIntoGroup(std::string const& hand, std::string const& bid);
handType findHandType(std::string const& hand);
void fillCounter(std::string const& hand, std::map<char, int>& counter);
bool comparator(std::pair<std::string, int> hand1, std::pair<std::string, int> hand2);
bool isNumber(char c);
bool isLessThan(char c1, char c2);

void printVec(std::vector<std::pair<std::string, int>> vec);

int calculateTotalWinnings();
void calculateHandTypeWinnings(std::vector<std::pair<std::string, int>> hands, int* rank, int* totalWinnings);

std::vector<std::pair<std::string, int>> fiveOfAKind = {};
std::vector<std::pair<std::string, int>> fourOfAKind = {};
std::vector<std::pair<std::string, int>> fullHouse = {};
std::vector<std::pair<std::string, int>> threeOfAKind = {};
std::vector<std::pair<std::string, int>> twoPair = {};
std::vector<std::pair<std::string, int>> onePair = {};
std::vector<std::pair<std::string, int>> highCard = {};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided!\n";
        return 0;
    }

    std::string s1;
    std::string s2;
    std::ifstream myFile;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        while (myFile.good()) {
            myFile >> s1;
            if (myFile.eof()) 
                break;
            myFile >> s2;
            //std::cout << s1 << " " << s2 << "\n";
            insertIntoGroup(s1, s2);
        }
    }

    std::sort(fiveOfAKind.begin(), fiveOfAKind.end(), comparator);
    std::cout << "Complete 1\n";
    
    std::sort(fourOfAKind.begin(), fourOfAKind.end(), comparator);
    std::cout << "Complete 2\n";

    std::sort(fullHouse.begin(), fullHouse.end(), comparator);
    std::cout << "Complete 3\n";

    std::sort(threeOfAKind.begin(), threeOfAKind.end(), comparator);
    std::cout << "Complete 4\n";

    std::sort(twoPair.begin(), twoPair.end(), comparator);
    std::cout << "Complete 5\n";

    std::sort(onePair.begin(), onePair.end(), comparator);
    std::cout << "Complete 6\n";

    std::sort(highCard.begin(), highCard.end(), comparator);
    std::cout << "Complete 7\n";

    int finalOutput = calculateTotalWinnings();
    std::cout << finalOutput << "\n";
}

void insertIntoGroup(std::string const& hand, std::string const& bid) {
    int numBid = std::stoi(bid);
    handType type = findHandType(hand);
    switch (type)
    {
    case FIVE_OF_A_KIND:
        fiveOfAKind.push_back({hand, numBid});
        break;
    case FOUR_OF_A_KIND:
        fourOfAKind.push_back({hand, numBid});
        break;
    case FULL_HOUSE:
        fullHouse.push_back({hand, numBid});
        break;
    case THREE_OF_A_KIND:
        threeOfAKind.push_back({hand, numBid});
        break;
    case TWO_PAIR:
        twoPair.push_back({hand, numBid});
        break;
    case ONE_PAIR:
        onePair.push_back({hand, numBid});
        break;
    case HIGH_CARD:
        highCard.push_back({hand, numBid});
        break;
    }
}

handType findHandType(std::string const& hand) {
    std::map<char, int> counter = {};
    fillCounter(hand, counter);

    if (counter[hand[0]] == 5) 
        return FIVE_OF_A_KIND;
    
    std::vector<int> values;
    for(std::map<char, int>::iterator it = counter.begin(); it != counter.end(); ++it) {
        values.push_back(it->second);
    }

    if (values.size() == 2) {
        if (values[0] == 4 or values[0] == 1)
            return FOUR_OF_A_KIND;
        else
            return FULL_HOUSE;
    }

    if (values.size() == 3) {
        for (int i : values) {
            if (i == 3)
                return THREE_OF_A_KIND;
            else if (i == 2)
                return TWO_PAIR;
        }
    }

    if (values.size() == 4)
        return ONE_PAIR;

    return HIGH_CARD;
}

void fillCounter(std::string const& hand, std::map<char, int>& counter) {
    for (char c : hand) {
        if (!counter.count(c))
            counter[c] = 0;
        counter[c]++;
    }
}

// return hand1 < hand2
bool comparator(std::pair<std::string, int> hand1, std::pair<std::string, int> hand2) {
    for (int i = 0; i < hand1.first.size(); i++) {
        if (hand1.first[i] != hand2.first[i])
            return isLessThan(hand1.first[i], hand2.first[i]);
    }
    return false;
}

bool isNumber(char c) {
    return c >= 48 and c <= 57;
}

// return c1 <= c2
bool isLessThan(char c1, char c2) {
    if (!isNumber(c1) and isNumber(c2))
        return false;
    else if (isNumber(c1) and !isNumber(c2))
        return true;
    else if (isNumber(c1) and isNumber(c2))
        return c1 <= c2;
    else   
        return letterStrength[c1] <= letterStrength[c2];
}

void printVec(std::vector<std::pair<std::string, int>> vec) {
    for (std::pair<std::string, int> i : vec) {
        std::cout << i.first << ", ";
    }
    std::cout << "\n";
}

int calculateTotalWinnings() {
    int rank = 1;
    int totalWinnings = 0;
    calculateHandTypeWinnings(highCard, &rank, &totalWinnings);
    calculateHandTypeWinnings(onePair, &rank, &totalWinnings);
    calculateHandTypeWinnings(twoPair, &rank, &totalWinnings);
    calculateHandTypeWinnings(threeOfAKind, &rank, &totalWinnings);
    calculateHandTypeWinnings(fullHouse, &rank, &totalWinnings);
    calculateHandTypeWinnings(fourOfAKind, &rank, &totalWinnings);
    calculateHandTypeWinnings(fiveOfAKind, &rank, &totalWinnings);
    return totalWinnings;
}

void calculateHandTypeWinnings(std::vector<std::pair<std::string, int>> hands, int* rank, int* totalWinnings) {
    for (std::pair<std::string, int> hand : hands) {
        *totalWinnings += (*rank) * hand.second;
        (*rank)++;
    }
}