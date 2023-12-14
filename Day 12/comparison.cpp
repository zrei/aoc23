#include <iostream>
#include <fstream>

int main() {
    std::ifstream reader1;
    reader1.open("output1.txt");
    std::ifstream reader2;
    reader2.open("output2.txt");
    std::string s1;
    std::string s2;
    int row = 0;
    while (reader1.good()) {
        getline(reader1, s1);
        getline(reader2, s2);
        if (reader1.eof())
            break;
        if (std::stoi(s1) != std::stoi(s2))
            std::cout << row << "\n";
        row++;
    }
}