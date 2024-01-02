#include <iostream>
#include <vector>
#include <fstream>
#include <map>

enum comparison {
    LESS_THAN,
    GREATER_THAN,
    AUTO_ACCEPT,
    AUTO_REJECT,
    AUTO_MOVE
};

enum outcome {
    ACCEPTED,
    REJECTED,
    MOVED
};

enum partValue {
    X = 'x',
    M = 'm',
    A = 'a',
    S = 's'
};

struct part {
    long long xValue;
    long long mValue;
    long long aValue;
    long long sValue;

    part(long long x, long long m, long long a, long long s) {
        xValue = x;
        mValue = m;
        aValue = a;
        sValue = s;
    }

    long long getPartValue(partValue type) {
        switch (type) {
            case X:
                return xValue;
            case M:
                return mValue;
            case A:
                return aValue;
            case S:
                return sValue;
        }
        return 0;
    }

    long long sumPartValues() {
        return xValue + mValue + aValue + sValue;
    }
}; 

struct condition {
    partValue type;
    comparison comparator;
    long long compareValue;
    std::string nextCheckerName;
    outcome o;

    condition(partValue t, comparison c, long long cv, std::string n, outcome o1) {
        type = t;
        comparator = c;
        compareValue = cv;
        nextCheckerName = n;
        o = o1;
    }

    bool checkCondition(part p, std::string& newCheckerName, outcome& o1) {
        bool passed = false;
        switch (comparator) {
            case GREATER_THAN:
                passed = p.getPartValue(type) > compareValue;
                if (passed) {
                    o1 = o;
                    newCheckerName = nextCheckerName;
                }
                return passed;
            case LESS_THAN:
                passed = p.getPartValue(type) < compareValue;
                if (passed) {
                    o1 = o;
                    newCheckerName = nextCheckerName;
                }
                return passed;
            case AUTO_ACCEPT:
                o1 = o;
                return true;
            case AUTO_REJECT:
                o1 = o;
                return true;
            case AUTO_MOVE:
                o1 = o;
                newCheckerName = nextCheckerName;
                return true;
        }
        return false;
    }
};

std::vector<part> parts = {};
std::map<std::string, std::vector<condition>> checkers = {};
std::string firstCheckerName = "in";

void processCheckers(std::string checkerInfo);
void processParts(std::string partInfo);
void check(part p, std::vector<condition> conditions, long long* finalOutput);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }
    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);

    int index = 0;

    if (myFile.is_open()) {
        while (myFile.good()) {
            myFile >> s;
            if (myFile.eof())
                break;
            if (s[0] == '{' and index == 0)
                index++;
            if (index == 0)
                processCheckers(s);
            else
                processParts(s);
        }
    }

    long long sum = 0;
    for (part p : parts) {
        check(p, checkers[firstCheckerName], &sum);
    }

    std::cout << sum << "\n";
}


void processParts(std::string partInfo) {
    int lp = 3;
    int rp = partInfo.find(',');
    long long xValue = std::stoi(partInfo.substr(lp, rp - lp));
    lp = 2;
    partInfo = partInfo.substr(rp + 1);
    rp = partInfo.find(',');
    long long mValue = std::stoi(partInfo.substr(lp, rp - lp));
    partInfo = partInfo.substr(rp + 1);
    rp = partInfo.find(',');
    long long aValue = std::stoi(partInfo.substr(lp, rp - lp));
    partInfo = partInfo.substr(rp + 1);
    long long sValue = std::stoi(partInfo.substr(lp, partInfo.size() - 1 - lp));
    parts.push_back(part(xValue, mValue, aValue, sValue));
}

void processCheckers(std::string checkerInfo) {
    int lp = 0;
    int rp = 0;
    while (checkerInfo[rp] != '{')
        rp++;
    std::string checkerName = checkerInfo.substr(lp, rp - lp);
    std::vector<condition> conditions = {};
    rp++;
    lp = rp;
    while (rp < checkerInfo.size()) {
        if (checkerInfo.substr(lp).find(',') == -1) {
            rp = checkerInfo.size() - 1;
            if (checkerInfo.substr(lp, rp - lp) == "R") {
                conditions.push_back(condition(X, AUTO_REJECT, 0, "", REJECTED));
            } else if (checkerInfo.substr(lp, rp - lp) == "A") {
                conditions.push_back(condition(X, AUTO_ACCEPT, 0, "", ACCEPTED));
            } else {
                conditions.push_back(condition(X, AUTO_MOVE, 0, checkerInfo.substr(lp, rp - lp), MOVED));
            }
            break;
        }
        long long compareValue;
        partValue partType;
        switch (checkerInfo[rp]) {
            case X:
                partType = X;
                break;
            case M:
                partType = M;
                break;
            case A:
                partType = A;
                break;
            case S:
                partType = S;
                break;
        }
        comparison c;
        rp++;
        lp = rp;
        if (checkerInfo[rp] == '<') {
            c = LESS_THAN;
        } else if (checkerInfo[rp] == '>') {
            c = GREATER_THAN;
        }
        rp++;
        lp = rp;
        while (checkerInfo[rp] != ':') {
            rp++;
        }
        compareValue = std::stoi(checkerInfo.substr(lp, rp - lp));
        rp++;
        lp = rp;
        while (checkerInfo[rp] != ',') {
            rp++;
        }
        if (checkerInfo.substr(lp, rp - lp) == "A") {
            conditions.push_back(condition(partType, c, compareValue, "", ACCEPTED));
        } else if (checkerInfo.substr(lp, rp - lp) == "R") {
            conditions.push_back(condition(partType, c, compareValue, "", REJECTED));
        } else {
            conditions.push_back(condition(partType, c, compareValue, checkerInfo.substr(lp, rp - lp), MOVED));
        }
        rp++;
        lp = rp;
    }
    checkers[checkerName] = conditions;
}

void check(part p, std::vector<condition> conditions, long long* finalOutput) {
    for (condition c : conditions) {
        std::string newCheckerName;
        outcome o;
        bool passed = c.checkCondition(p, newCheckerName, o);
        if (passed) {
            switch (o) {
                case ACCEPTED:
                    *finalOutput += p.sumPartValues();
                case REJECTED:
                    break;
                case MOVED:
                    check(p, checkers[newCheckerName], finalOutput);
                    break;
            }
            break;
        }
    }
}