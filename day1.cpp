#include <iostream>
#include <fstream>
#include <string>

int getLineSum(std::string const& line);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Did not provide file name!\n";
    return 0;
  }

  std::ifstream myFile;
  myFile.open(argv[1]);
  std::string s;
  int finalOutput = 0;

  if (myFile.is_open()) {
    while (myFile.good()) {
      myFile >> s;
      if (myFile.eof()) 
         break;
      finalOutput += getLineSum(s);
    }
  }

  std::cout << finalOutput << "\n";
  return 0;
}

int getLineSum(std::string const& line) {
  int first = -1;
  int last = -1;
  for (int i = 0; i < line.size(); i++) {
    if (line[i] >= 48 and line[i] <= 57) {
      if (first == -1) {
        first = line[i] - '0';
      }
      last = line[i] - '0';
    }
  }
  return first * 10 + last;
}
