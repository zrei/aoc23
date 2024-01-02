#include <iostream>
#include "z3++.h"
#include <fstream>
#include <string>
#include <vector>

using namespace z3;

// this code outputs the initial x, y and z values of the stone.
// They are summed in another program.

#define MAIN_SEPARATOR '@'
#define X 0
#define Y 1
#define Z 2

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
        std::cout << this->xPos << ", " << this->yPos << ", " << this->zPos << ", " << this->xVel << ", " << this->yVel << ", " << this->zVel << "\n";
    }
};

std::vector<hailstone> hailstones = {};

void processLine(std::string s);
std::vector<long long> processValues(std::string s);
void solve();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        if (myFile.good()) {
            for (int i = 0; i < 3; i ++) {
                getline(myFile, s);
                if (myFile.eof())
                    break;
                processLine(s);
            }
        }
    }

    solve();
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

void solve() {
    config cfg;
    cfg.set("auto_config", true);
    context c(cfg);

    expr sx = c.real_const("sx");
    expr sy = c.real_const("sy");
    expr sz = c.real_const("sz");
    expr vx = c.real_const("vx");
    expr vy = c.real_const("vy");
    expr vz = c.real_const("vz");
    expr t1 = c.real_const("t1");
    expr t2 = c.real_const("t2");
    expr t3 = c.real_const("t3");

    std::vector<expr> times = {t1, t2, t3};  
    solver s(c);

    for (int i = 0; i < hailstones.size(); i++) {
        s.add(c.int_val(hailstones[i].xPos) + times[i] * c.int_val(hailstones[i].xVel) == sx + times[i] * vx); 
        s.add(c.int_val(hailstones[i].yPos) + times[i] * c.int_val(hailstones[i].yVel) == sy + times[i] * vy); 
        s.add(c.int_val(hailstones[i].zPos) + times[i] * c.int_val(hailstones[i].zVel) == sz + times[i] * vz); 
    }
    
    std::cout << s.check() << "\n";
    model m = s.get_model();
    set_param("pp.decimal", true); // set decimal notation
    std::cout << "model in decimal notation\n";
    std::cout << m << "\n";
    set_param("pp.decimal", false); // disable decimal notation
}