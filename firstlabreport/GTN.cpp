#include "GTN.h"
#include <queue>
#include <algorithm>
#include "utilities.h"
#include <string>
#include <format>

GTN::GTN() {}

bool GTN::isSuitable(int diameter) const {
    return std::find(suitableDiameters.begin(), suitableDiameters.end(), diameter) != suitableDiameters.end();
}

bool GTN::ConnectCS(int startCSID, int endCSID, int diameter) {
    if (CSPackage.empty() || pipePackage.empty()) {
        std::cout << "No pipes or compressor stations added yet.\n";
        return false;
    }

    if (CSPackage.find(startCSID) == CSPackage.end() || CSPackage.find(endCSID) == CSPackage.end()) {
        std::cout << "Invalid CS IDs provided.\n";
        return false;
    }

    int suitablePipeID = -1;
    for (auto& pipe : pipePackage) {
        if (!pipe.second.isConnected() && pipe.second.GetRepairingStatus() && isSuitable(diameter) && pipe.second.GetDiameter() == diameter) {
            suitablePipeID = pipe.first;
            break;
        }
    }

    if (suitablePipeID == -1) {
        std::cout << "No available pipes with the specified diameter or all pipes are already in use.\n";
        std::cout << "Creating a new pipe...\n";

        while (true) {
            std::cout << "Enter pipe diameter (500, 700, 1000, or 1400 mm): ";
            std::cin >> diameter;

            if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
                break;
            }
            else {
                std::cout << "Invalid diameter! Please enter a valid diameter (500, 700, 1000, or 1400 mm).\n";
            }
        }

        Pipe suitablePipe;
        suitablePipe.CreateSuitablePipe(pipePackage, diameter);

        suitablePipeID = suitablePipe.GetID();
    }

    graph[startCSID][endCSID] = suitablePipeID;
    std::cout << "Connection established between CS " << startCSID
        << " and CS " << endCSID << " using pipe ID " << suitablePipeID << ".\n";

    pipePackage[suitablePipeID].setFree();
    return true;
}

void GTN::DeleteGTN() {
    graph = {};
}

std::vector<int> GTN::TopologicalSort() const {
    std::unordered_map<int, int> inDegree;
    std::vector<int> sortedNodes;            

    for (const auto& [node, edges] : graph) {
        if (inDegree.find(node) == inDegree.end()) {
            inDegree[node] = 0;
        }

        for (const auto& [adjNode, pipeID] : edges) {
            inDegree[adjNode]++;
            if (inDegree.find(adjNode) == inDegree.end()) {
                inDegree[adjNode] = 0;
            }
        }
    }

    std::queue<int> zeroInDegree;
    for (const auto& [node, degree] : inDegree) {
        if (degree == 0) {
            zeroInDegree.push(node);
        }
    }

    while (!zeroInDegree.empty()) {
        int node = zeroInDegree.front();
        zeroInDegree.pop();
        sortedNodes.push_back(node);

        auto it = graph.find(node);
        if (it != graph.end()) {
            for (const auto& [adjNode, pipeID] : it->second) {
                inDegree[adjNode]--;
                if (inDegree[adjNode] == 0) {
                    zeroInDegree.push(adjNode);
                }
            }
        }
    }

    if (sortedNodes.size() != inDegree.size()) {
        std::cout << "Cycle detected in the graph.\n";
        return {};
    }

    std::cout << "Topological Order: ";
    for (int node : sortedNodes) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    return sortedNodes;
}

std::unordered_map<int, Pipe>& GTN::GetPipePackage() {
    return pipePackage;
}

std::unordered_map<int, CS>& GTN::GetCSPackage() {
    return CSPackage;
}

std::unordered_map<int, std::unordered_map<int, int>>& GTN::getGraphMat() {
    return graph;
}

void GTN::DisplayNetwork() const {
    for (const auto& [startCSID, edges] : graph) {
        for (const auto& [endCSID, pipeID] : edges) {

            auto pipeIt = pipePackage.find(pipeID);
            if (pipeIt != pipePackage.end()) {
                std::cout << "CS " << startCSID << " -> CS " << endCSID
                    << " via Pipe with ID " << pipeID
                    << " (" << pipeIt->second.GetName() << ")" << '\n';
            }
        }
    }
}
