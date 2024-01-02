#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// first step: sort the blocks by their lower z value
// drop the blocks, maintaining the max z value at each x and y value to know when to stop dropping a block
// sort the blocks by their lower z value again before finding which blocks a block is supporting

// then for each block, check whether it's supported blocks are only supported by one block

#define SEPARATOR '~'

int globalId = 0;

enum axis {
    X,
    Y,
    Z
};

std::vector<std::vector<int>> maxZValueMatrix = {};

class jengaBlock {
public:
    int id;
    
    int firstRangeX;
    int firstRangeY;
    int firstRangeZ;
    
    int secondRangeX;
    int secondRangeY;
    int secondRangeZ;

    std::vector<jengaBlock*> supportedBlocks;
    axis side;
    long long numSupportingBlocks;

    jengaBlock(int firstRangeX, int firstRangeY, int firstRangeZ, int secondRangeX, int secondRangeY, int secondRangeZ, std::vector<jengaBlock*> supportedBlocks, int id, axis side) {
        this->firstRangeX = firstRangeX;
        this->firstRangeY = firstRangeY;
        this->firstRangeZ = firstRangeZ;
        
        this->secondRangeX = secondRangeX;
        this->secondRangeY = secondRangeY;
        this->secondRangeZ = secondRangeZ;

        this->supportedBlocks = supportedBlocks;
        this->id = id;
        this->side = side;
        numSupportingBlocks = 0;
    }

    void addSupporting() {
        numSupportingBlocks++;
    }

    void addSupported(jengaBlock* block) {
        supportedBlocks.push_back(block);
    }

    bool checkCanBeDisintegrated() {
        for (jengaBlock* b : supportedBlocks) {
            if (b->checkOnlySupporter())
                return false;
        }
        return true;
    }

    // should only be called if you know this block is supported by you!
    bool checkOnlySupporter() {
        return numSupportingBlocks == 1;
    }

    bool isSupporting(jengaBlock* otherBlock) {
        if (side == X and side == otherBlock->side) {
            bool xWithin = (otherBlock->firstRangeX >= firstRangeX and otherBlock->firstRangeX <= secondRangeX) or (otherBlock->secondRangeX >= firstRangeX and otherBlock->secondRangeX <= secondRangeX) or (firstRangeX >= otherBlock->firstRangeX and firstRangeX <= otherBlock->secondRangeX) or (secondRangeX >= otherBlock->firstRangeX and secondRangeX <= otherBlock->secondRangeX);
            return xWithin and otherBlock->firstRangeY == firstRangeY;
        } else if (side == Y and side == otherBlock->side) {
            bool yWithin = (otherBlock->firstRangeY >= firstRangeY and otherBlock->firstRangeY <= secondRangeY) or (otherBlock->secondRangeY >= firstRangeY and otherBlock->secondRangeY <= secondRangeY) or (firstRangeY >= otherBlock->firstRangeY and firstRangeY <= otherBlock->secondRangeY) or (secondRangeY >= otherBlock->firstRangeY and secondRangeY <= otherBlock->secondRangeY);
            return yWithin and otherBlock->firstRangeX == firstRangeX;
        } else if (side == Z and side == otherBlock->side) {
            return firstRangeX == otherBlock->firstRangeX and firstRangeY == otherBlock->firstRangeY;
        } else if (side == X and otherBlock->side == Y) {
            bool xWithin = otherBlock->firstRangeX >= firstRangeX and otherBlock->firstRangeX <= secondRangeX;
            bool yWithin = firstRangeY >= otherBlock->firstRangeY and firstRangeY <= otherBlock->secondRangeY;
            return xWithin and yWithin;
        } else if (side == X and otherBlock->side == Z) {
            bool xWithin = otherBlock->firstRangeX >= firstRangeX and otherBlock->firstRangeX <= secondRangeX;
            return xWithin and otherBlock->firstRangeY == firstRangeY;
        } else if (side == Y and otherBlock->side == X) {
            bool xWithin = firstRangeX >= otherBlock->firstRangeX and firstRangeX <= otherBlock->secondRangeX;
            bool yWithin = otherBlock->firstRangeY >= firstRangeY and otherBlock->firstRangeY <= secondRangeY;
            return xWithin and yWithin;
        } else if (side == Y and otherBlock->side == Z) {
            bool yWithin = otherBlock->firstRangeY >= firstRangeY and otherBlock->firstRangeY <= secondRangeY;
            return yWithin and otherBlock->firstRangeX == firstRangeX;
        } else if (side == Z and otherBlock->side == X) {
            bool xWithin = firstRangeX >= otherBlock->firstRangeX and firstRangeX <= otherBlock->secondRangeX;
            return xWithin and otherBlock->firstRangeY == firstRangeY;
        } else if (side == Z and otherBlock->side == Y) {
            bool yWithin = firstRangeY >= otherBlock->firstRangeY and firstRangeY <= otherBlock->secondRangeY;
            return yWithin and otherBlock->firstRangeX == firstRangeX;
        }
        return false;
    }

    void dropBlock() {
        if (side == X) {
            int dropHeight = 0;
            for (int x = firstRangeX; x <= secondRangeX; x++) {
                dropHeight = std::max(dropHeight, maxZValueMatrix[x][firstRangeY]);
            }
            dropHeight++;
            dropBlock(firstRangeZ - dropHeight);
            for (int x = firstRangeX; x <= secondRangeX; x++) {
                maxZValueMatrix[x][firstRangeY] = std::max(maxZValueMatrix[x][firstRangeY], secondRangeZ);
            }
        } else if (side == Y) {
            int dropHeight = 0;
            for (int y = firstRangeY; y <= secondRangeY; y++) {
                dropHeight = std::max(dropHeight, maxZValueMatrix[firstRangeX][y]);
            }
            dropHeight++;
            dropBlock(firstRangeZ - dropHeight);
            for (int y = firstRangeY; y <= secondRangeY; y++) {
                maxZValueMatrix[firstRangeX][y] = std::max(maxZValueMatrix[firstRangeX][y], secondRangeZ);
            }
        } else if (side == Z) {
            int dropHeight = maxZValueMatrix[firstRangeX][firstRangeY] + 1;
            dropBlock(firstRangeZ - dropHeight);
            maxZValueMatrix[firstRangeX][firstRangeY] = std::max(maxZValueMatrix[firstRangeX][firstRangeY], secondRangeZ);
        }
    }

    void dropBlock(int amt) {
        firstRangeZ -= amt;
        secondRangeZ -= amt;
    }

    int getLowerZ() {
        return firstRangeZ;
    }
};

std::vector<jengaBlock*> blocks = {};
int minX = 0;
int minY = 0;
int maxX = 0;
int maxY = 0;

void processLine(std::string s);
bool comparator(jengaBlock* jenga1, jengaBlock* jenga2);
void dropBlocks();
void createMatrix();
void findSupporters();
long long numBricksDisintegrated();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }

    std::string s;
    std::ifstream myFile;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        while (myFile.good()) {
            std::getline(myFile, s);
            if (myFile.eof()) 
                break;
            processLine(s);
        }
    }
    std::cout << "Minimum: " << minX << ", " << minY << ", Maximum: " << maxX << ", " << maxY << "\n";
    std::sort(blocks.begin(), blocks.end(), comparator);
    createMatrix();
    dropBlocks();  
    std::sort(blocks.begin(), blocks.end(), comparator);
    findSupporters();  
    std::cout << "Number of bricks that can be disintegrated: " << numBricksDisintegrated() << "\n";
}

void processLine(std::string s) {
    int commaIdx = s.find(',');
    int firstX = std::stoi(s.substr(0, commaIdx));
    s = s.substr(commaIdx + 1);
    commaIdx = s.find(',');
    int firstY = std::stoi(s.substr(0, commaIdx));
    s = s.substr(commaIdx + 1);
    int sepIdx = s.find(SEPARATOR);
    int firstZ = std::stoi(s.substr(0, sepIdx));
    s = s.substr(sepIdx + 1);
    commaIdx = s.find(',');
    int secondX = std::stoi(s.substr(0, commaIdx));
    maxX = std::max(secondX, maxX);
    s = s.substr(commaIdx + 1);
    commaIdx = s.find(',');
    int secondY = std::stoi(s.substr(0, commaIdx));
    maxY = std::max(secondY, maxY);
    s = s.substr(commaIdx + 1);
    int secondZ = std::stoi(s);
    axis side;
    if (firstX != secondX)
        side = X;
    else if (firstY != secondY) 
        side = Y;
    else
        side = Z;
    std::vector<jengaBlock*> supportedBlocks = {};
    blocks.push_back(new jengaBlock(firstX, firstY, firstZ, secondX, secondY, secondZ, supportedBlocks, globalId, side));
    globalId++;
}

// return jenga1's z < jenga2's z
bool comparator(jengaBlock* jenga1, jengaBlock* jenga2) {
    return (*jenga1).getLowerZ() < (*jenga2).getLowerZ();
}

void createMatrix() {
    maxZValueMatrix.clear();
    for (int j = 0; j <= maxX; j++) {
        std::vector<int> temp = {};
        for (int i = 0; i <= maxY; i++)
            temp.push_back(0);
        maxZValueMatrix.push_back(temp);
    }
}

void dropBlocks() {
    for (jengaBlock* b : blocks) {
        (*b).dropBlock();
    }
}

void findSupporters() {
    for (int i = 0; i < blocks.size(); i++) {
        for (int j = i + 1; j < blocks.size(); j++) {
            if (blocks[j]->firstRangeZ <= blocks[i]->secondRangeZ)
                continue;
            if (blocks[j]->firstRangeZ > blocks[i]->secondRangeZ + 1)
                break;

            if (blocks[i]->isSupporting(blocks[j])) {
                blocks[i]->addSupported(blocks[j]);
                blocks[j]->addSupporting();
            }
        }
    }
}

long long numBricksDisintegrated() {
    long long num = 0;
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i]->checkCanBeDisintegrated()) {
            num++;
        }
    }
    return num;
}