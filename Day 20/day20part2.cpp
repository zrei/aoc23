/* this code simply outputs the number of button presses required 
for all conjunction modules to have outputted their first high
pulse. The LCM of the number of button presses for the four 
cojunction modules outputting to the single conjunction module
that outputs to rx is calculated manually. */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define BUTTON_NAME "button"
#define BROADCASTER_NAME "broadcaster"

long long cycleNum = 0;

enum pulseType {
    HIGH,
    LOW
};

enum flipFlopState {
    OFF,
    ON
};

class modules {
protected:
    std::vector<modules*> connections = {};

public:
    std::string moduleName;

    modules(std::string moduleName) {
        this->moduleName = moduleName;
    }

    virtual void transmit(pulseType p, std::string inputName) {
        for (modules* m : connections) {
            (*m).transmit(p, moduleName);
        }
    }

    virtual void addDestination(modules* m) {
        connections.push_back(m);
        m->addInput(moduleName);
    }

    virtual void addInput(std::string moduleName) {
    
    }
};

struct pulseInfo {
    modules* module;
    pulseType type;
    std::string inputName;

    pulseInfo(modules* module, pulseType type, std::string inputName) {
        this->module = module;
        this->type = type;
        this->inputName = inputName;
    }
};

std::deque<pulseInfo> pulseQueue = {};

class buttonModule : public modules {
public:

    buttonModule(std::string moduleName) : modules(moduleName) {

    }

    void transmit(pulseType p, std::string inputName) {
        for (modules* m : connections) {
            pulseQueue.push_back(pulseInfo(m, LOW, moduleName));
        }
    }
};

class broadcaster : public modules {
public:

    broadcaster(std::string moduleName) : modules(moduleName) {

    }

    void transmit(pulseType p, std::string inputName) {
        for (modules* m : connections) {
            pulseQueue.push_back(pulseInfo(m, p, moduleName));
        }
    }
};

class flipFlopModule : public modules {
    flipFlopState state = OFF;

public:

    flipFlopModule(std::string moduleName) : modules(moduleName) {

    }

    void transmit(pulseType p, std::string inputName) {
        if (p == HIGH)
            return;
        state = state == ON ? OFF : ON;
        if (state == ON) {
            for (modules* m : connections) {
                pulseQueue.push_back(pulseInfo(m, HIGH, moduleName));
            }
        } else if (state == OFF) {
            for (modules* m : connections) {
                pulseQueue.push_back(pulseInfo(m, LOW, moduleName));
            }
        }

    }
};

class conjunctionModule : public modules {
    std::map<std::string, pulseType> inputs;
    bool firstHigh;
    long long firstHighCycle;

    bool checkInputsAllHigh() {
        for (std::map<std::string, pulseType>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
            if (it->second != HIGH)
                return false;
        }
        return true;
    }

    void updateInputs(std::string name, pulseType p) {
        inputs[name] = p;
    }
public:

    conjunctionModule(std::string moduleName) : modules(moduleName) {
        inputs = {};
        firstHigh = false;
        firstHighCycle = 0;
    }

    void addInput(std::string moduleName) {
        inputs[moduleName] = LOW;
    }

    void transmit(pulseType p, std::string inputName) {
        inputs[inputName] = p;
        if (checkInputsAllHigh()) {
            for (modules* m : connections) {
                pulseQueue.push_back(pulseInfo(m, LOW, moduleName));
            }
        } else {
            for (modules* m : connections) {
                pulseQueue.push_back(pulseInfo(m, HIGH, moduleName));
                if (!firstHigh)
                    firstHighCycle = cycleNum;
                firstHigh = true;
            }
        }
    }

    void printFirstHighNumber() {
        std::cout << "First cycle where a high was outputted for conjunction module " << moduleName << " was " << firstHighCycle << "\n";
    }

    bool hasOutputtedFirstHigh() {
        return firstHigh;
    }
};

std::vector<conjunctionModule*> conjunctionModules = {};

class outputModule : public modules {
    
public:

    outputModule(std::string moduleName) : modules(moduleName) {
    
    }

    void addDestination(modules* m) {

    }

    void transmit(pulseType p, std::string inputName) {
        
    }

};

outputModule* finalModule;

std::map<std::string, modules*> m = {};

void addDestinations(std::string s);
void processModuleName(std::string s);
void helper();
bool checkAllConjunctions();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }

    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);

    if (myFile.is_open())
    {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            processModuleName(s);
        }
    }
    myFile.close();
    std::cout << "Done with parsing module names and types!\n";

    myFile.open(argv[1]);
    if (myFile.is_open())
    {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            addDestinations(s);
        }
    }
    myFile.close();
    std::cout << "Done with adding destinations!\n";

    m[BUTTON_NAME] = new buttonModule(BUTTON_NAME);
    (*m[BUTTON_NAME]).addDestination(m[BROADCASTER_NAME]);

    helper();
}


void processModuleName(std::string s) {
    int spaceIndex = s.find(' ');
    char type = s[0];
    std::string moduleName;
    conjunctionModule* temp;
    switch (type) {
        case '%':
            moduleName = s.substr(1, spaceIndex - 1);
            m[moduleName] = new flipFlopModule(moduleName);
            break;
        case '&':
            moduleName = s.substr(1, spaceIndex - 1);
            temp = new conjunctionModule(moduleName);
            m[moduleName] = temp;
            conjunctionModules.push_back(temp);
            break;
        default:
            moduleName = s.substr(0, spaceIndex);
            m[moduleName] = new broadcaster(moduleName);
            break;
    }
}

void addDestinations(std::string s) {
    int spaceIndex = s.find(' ');
    char type = s[0];
    std::string moduleName;
    std::vector<std::string> destinationNames = {};
    std::string destinationSplice = s.substr(spaceIndex + 4);
    int lp = 0;
    int rp = 0;
    while (rp < destinationSplice.size()) {
        if (destinationSplice[rp] == ',') {
            destinationNames.push_back(destinationSplice.substr(lp, rp - lp));
            lp = rp + 2;
            rp += 2;
        } else {
            rp++;
        }
    }
    destinationNames.push_back(destinationSplice.substr(lp, rp - lp));
    
    switch (type) {
        case '%':
            moduleName = s.substr(1, spaceIndex - 1);
            for (std::string s : destinationNames) {
                if (!m.count(s)) {
                    finalModule = new outputModule(s);
                    (*m[moduleName]).addDestination(finalModule);
                }
                else
                    (*m[moduleName]).addDestination(m[s]);
            }
            break;
        case '&':
            moduleName = s.substr(1, spaceIndex - 1);
            for (std::string s : destinationNames) {
                if (!m.count(s)) {
                    finalModule = new outputModule(s);
                    (*m[moduleName]).addDestination(finalModule);
                }
                else {
                    (*m[moduleName]).addDestination(m[s]);
                }
            }
            break;
        default:
            moduleName = s.substr(0, spaceIndex);
            for (std::string s : destinationNames) {
                if (!m.count(s)) {
                    finalModule = new outputModule(s);
                    (*m[moduleName]).addDestination(finalModule);
                }
                else
                    (*m[moduleName]).addDestination(m[s]);
            }
            break;
    }
}

void helper() { 
    while (!checkAllConjunctions()) {
        cycleNum++;
        pulseQueue.push_back(pulseInfo(m[BUTTON_NAME], LOW, ""));
        while (!pulseQueue.empty()) {
            pulseInfo pulse = pulseQueue.front();
            pulseQueue.pop_front();
            (*(pulse.module)).transmit(pulse.type, pulse.inputName);
        }
    }
    for (conjunctionModule* c : conjunctionModules) 
        c->printFirstHighNumber();
}

bool checkAllConjunctions() {
    for (conjunctionModule* c : conjunctionModules)
        if (!c->hasOutputtedFirstHigh())
            return false;
    return true;
}