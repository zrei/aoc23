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

struct range {
    std::pair<long long, long long> xRange;
    std::pair<long long, long long> mRange;
    std::pair<long long, long long> aRange;
    std::pair<long long, long long> sRange;

    range() {
        xRange = {1, 4000};
        mRange = {1, 4000};
        aRange = {1, 4000};
        sRange = {1, 4000};
    }

    range(std::pair<long long, long long> x, std::pair<long long, long long> m, std::pair<long long, long long> a, std::pair<long long, long long> s) {
        xRange = x;
        mRange = m;
        aRange = a;
        sRange = s;
    }

    range minCapX(int newXMin) {
        return range({newXMin, xRange.second}, mRange, aRange, sRange);
    }

    range maxCapX(int newXMax) {
        return range({xRange.first, newXMax}, mRange, aRange, sRange);
    }

    range minCapM(int newMMin) {
        return range(xRange, {newMMin, mRange.second}, aRange, sRange);
    }

    range maxCapM(int newMMax) {
        return range(xRange, {mRange.first, newMMax}, aRange, sRange);
    }

    range minCapA(int newAMin) {
        return range(xRange, mRange, {newAMin, aRange.second}, sRange);
    }

    range maxCapA(int newAMax) {
        return range(xRange, mRange, {aRange.first, newAMax}, sRange);
    }

    range minCapS(int newSMin) {
        return range(xRange, mRange, aRange, {newSMin, sRange.second});
    }

    range maxCapS(int newSMax) {
        return range(xRange, mRange, aRange, {sRange.first, newSMax});
    }

    long long numCombinations() {
        return (xRange.second - xRange.first + 1) * (mRange.second - mRange.first + 1) * (aRange.second - aRange.first + 1) * (sRange.second - sRange.first + 1);
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

    range affectRange(range r) {
        switch (type) {
            case X:
                if (comparator == LESS_THAN) {
                    return r.maxCapX(compareValue - 1);
                } else if (comparator == GREATER_THAN) {
                    return r.minCapX(compareValue + 1);
                }
                break;
            case M:
                if (comparator == LESS_THAN) {
                    return r.maxCapM(compareValue - 1);
                } else if (comparator == GREATER_THAN) {
                    return r.minCapM(compareValue + 1);
                }
                break;
            case A:
                if (comparator == LESS_THAN) {
                    return r.maxCapA(compareValue - 1);
                } else if (comparator == GREATER_THAN) {
                    return r.minCapA(compareValue + 1);
                }
                break;
            case S:
                if (comparator == LESS_THAN) {
                    return r.maxCapS(compareValue - 1);
                } else if (comparator == GREATER_THAN) {
                    return r.minCapS(compareValue + 1);
                }
                break;
        }
        return r;
    }

    range oppositeRange(range r) {
        switch (type) {
            case X:
                if (comparator == LESS_THAN) {
                    return r.minCapX(compareValue);
                } else if (comparator == GREATER_THAN) {
                    return r.maxCapX(compareValue);
                }
                break;
            case M:
                if (comparator == LESS_THAN) {
                    return r.minCapM(compareValue);
                } else if (comparator == GREATER_THAN) {
                    return r.maxCapM(compareValue);
                }
                break;
            case A:
                if (comparator == LESS_THAN) {
                    return r.minCapA(compareValue);
                } else if (comparator == GREATER_THAN) {
                    return r.maxCapA(compareValue);
                }
                break;
            case S:
                if (comparator == LESS_THAN) {
                    return r.minCapS(compareValue);
                } else if (comparator == GREATER_THAN) {
                    return r.maxCapS(compareValue);
                }
                break;
        }
        return r;
    }
};

std::map<std::string, std::vector<condition>> checkers = {};
std::string firstCheckerName = "in";

void processCheckers(std::string checkerInfo);
void helper(std::vector<condition> conditions, long long* finalOutput, range r);

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
                break;
            processCheckers(s);
        }
    }

    long long totalCombis = 0;
    helper(checkers[firstCheckerName], &totalCombis, range());
    std::cout << totalCombis << "\n";
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

void helper(std::vector<condition> conditions, long long* finalOutput, range r) {
    for (condition c : conditions) {
        if (c.comparator == AUTO_ACCEPT) {
            *finalOutput += r.numCombinations();
            return;
        } else if (c.comparator == AUTO_REJECT) {
            return;
        } else if (c.comparator == AUTO_MOVE) {
            helper(checkers[c.nextCheckerName], finalOutput, r);
        } else if (c.o == ACCEPTED) {
            *finalOutput += c.affectRange(r).numCombinations();
        } else if (c.o == MOVED) {
            helper(checkers[c.nextCheckerName], finalOutput, c.affectRange(r));
        }
        r = c.oppositeRange(r);
    }
}