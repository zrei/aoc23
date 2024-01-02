#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

#define SEPARATOR ':'

std::vector<std::pair<int, int>> initialEdges = {};
std::vector<std::pair<int, int>> edges = {};
std::map<std::string, int> createdVertices = {};

struct nodeCollection {
    std::vector<int> nodeIds;

    nodeCollection(std::vector<int> nodeIds) {
        this->nodeIds = nodeIds;
    }

    bool containsVertice(int vertice) {
        for (int v : nodeIds)
            if (v == vertice)
                return true;
        return false;
    }
};

std::vector<nodeCollection*> initialNodes = {};
std::vector<nodeCollection*> nodes = {};

int globalCounter = 0;
int numVertices;

void processLine(std::string s);
void processConnections(std::string s, int nodeNum);
void createNode(std::string s);
long long kargers();
nodeCollection* findSpecificSet(int vertice);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Filename not provided\n";
        return 0;
    }

    std::ifstream myFile;
    std::string s;
    myFile.open(argv[1]);

    if (myFile.is_open()) {
        while (myFile.good()) {
            getline(myFile, s);
            if (myFile.eof())
                break;
            processLine(s);
        }
    }
    numVertices = globalCounter;

    std::cout << kargers() << "\n";
}

void processLine(std::string s) {
    int sepIndex = s.find(SEPARATOR);
    std::string nodeName = s.substr(0, sepIndex);
    createNode(nodeName);
    processConnections(s.substr(sepIndex + 2), createdVertices[nodeName]);
}

void processConnections(std::string s, int nodeNum) {
    int lp = 0;
    int rp = 0;
    std::string currNodeName;
    while (rp < s.size()) {
        if (s[rp] == ' ') {
            currNodeName = s.substr(lp, rp - lp);
            createNode(currNodeName);
            initialEdges.push_back({nodeNum, createdVertices[currNodeName]});
            lp = rp + 1;
            rp++;
        } else {
            rp++;
        }
    }
    currNodeName = s.substr(lp, rp - lp);
    createNode(currNodeName);
    initialEdges.push_back({nodeNum, createdVertices[currNodeName]});
}

void createNode(std::string s) {
    if (!createdVertices.count(s)) {
        std::vector<int> temp = {globalCounter};
        initialNodes.push_back(new nodeCollection(temp));
        createdVertices[s] = globalCounter;
        globalCounter++;
    }
}

long long kargers() {
    bool done = false;
    while (!done) {
        nodes = initialNodes;
        edges = initialEdges;
        while (nodes.size() > 2) {
            int i = rand() % edges.size();
            std::pair<int, int> edge = edges[i];
            nodeCollection* firstCollection = findSpecificSet(edge.first);
            nodeCollection* secondCollection = findSpecificSet(edge.second);

            if (firstCollection == secondCollection)
                continue;

            std::vector<int> temp = {};
            for (int v : firstCollection->nodeIds)
                temp.push_back(v);
            for (int v : secondCollection->nodeIds)
                temp.push_back(v);
            nodeCollection* newCollection = new nodeCollection(temp);
            std::vector<nodeCollection*> newNodes = {};
            for (nodeCollection* n : nodes) {
                if (n == firstCollection or n == secondCollection)
                    continue;
                newNodes.push_back(n);
            }
            newNodes.push_back(newCollection);
            nodes = newNodes;

            std::vector<std::pair<int, int>> newEdges = {};
            for (std::pair<int, int> e : edges) {
                if (e == edge)
                    continue;
                if (newCollection->containsVertice(e.first) and newCollection->containsVertice(e.second))   
                    continue;
                newEdges.push_back(e);
            }
            edges = newEdges;
        }

        if (edges.size() == 3)
            done = true;

        std::cout << "Finished a cycle\n";
        std::cout << "Edges size: " << edges.size() << "\n";
    }

    return nodes[0]->nodeIds.size() * nodes[1]->nodeIds.size();
}

nodeCollection* findSpecificSet(int vertice) {
    for (nodeCollection* n : nodes) {
        for (int i : n->nodeIds) {
            if (i == vertice)
                return n;
        }
    }
    return nodes.front();
}