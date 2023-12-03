#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class Solution {
  public: 
    std::map<int, int> appearances = {};
    std::vector<std::string> numbers = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    int getLineSum(std::string const& line) {
      int lp = 0;
      int rp = line.size() - 1;
      int first = -1;
      int last = -1;
      while (first == -1) {
        if (line[lp] >= 48 and line[lp] <= 57) {
          first = line[lp] - '0';
        } else if (this->appearances.count(lp)) {
          first = (*this).appearances[lp];
        }
        lp++;
      }
      while (last == -1) {
        if (line[rp] >= 48 and line[rp] <= 57) {
          last = line[rp] - '0';
        } else if (this->appearances.count(rp)) {
          last = this->appearances[rp];
        }
        rp--;
      }
      /*for (int i = 0; i < line.size(); i++) {
        if (line[i] >= 48 and line[i] <= 57) {
          if (first == -1) {
            first = line[i] - '0';
          }
          last = line[i] - '0';
        }
      }*/
      return first * 10 + last;
    }

    void getNumberAppearances(std::string const& line, int number, std::vector<std::string> const& matchingStrings) {
      std::string newString = "";
      std::string matcher = matchingStrings[number - 1];
      int lp = 0;
      int rp = 0;
      int stringLength = matcher.size();
      
      while (rp < line.size()) {
        if (line[rp] != matcher[rp - lp]) {
          lp = rp;
          
          while (lp < line.size() and line[lp] != matcher[0])
            lp += 1;
            
          if (lp >= line.size())
            break;
          rp = lp;
        } 
        if ((rp - lp + 1) == stringLength) {
          this->appearances[lp] = number;
          lp = rp + 1;
        }
        rp += 1;
      }
    }

    int processLine(std::string const& line) {
      this->appearances.clear();
      for (int i = 1; i < 10; i++)
        getNumberAppearances(line, i, numbers);
      return getLineSum(line);
    }
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Did not provide file name!\n";
    return 0;
  }

  std::ifstream myFile;
  myFile.open(argv[1]);
  std::string s;
  int finalOutput = 0;

  Solution sol;

  if (myFile.is_open()) {
    while (myFile.good()) {
      myFile >> s;
      if (myFile.eof())
         break;
      std::cout << "Original string: " << s << "\n";
      int lineSum = sol.processLine(s);
      std::cout << "Line sum: " << lineSum << "\n";
      finalOutput += lineSum;
    }
  }

  std::cout << finalOutput << "\n";
  return 0;
}


