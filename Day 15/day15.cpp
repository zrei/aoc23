#include <iostream>
#include <fstream>
#include <string>

#define SEPARATOR ','

void processLine(std::string const& s, long long* finalOutput);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "File name not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    getline(myFile, s);
    long long finalOutput = 0;
    processLine(s, &finalOutput);
    std::cout << finalOutput << "\n";
}

void processLine(std::string const& s, long long* finalOutput) {
    int rp = 0;
    long long currNum = 0;
    while (rp < s.size()) {
        if (s[rp] == SEPARATOR) {
            *finalOutput += currNum;
            currNum = 0;
        } else {
            currNum += s[rp];
            currNum *= 17;
            currNum = currNum % 256;
        }
        rp++;
    }
    *finalOutput += currNum;
}