#pragma once
#include <unordered_map>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "CS.h"

class GTN {
private:
    std::unordered_map<int, Pipe> pipePackage;
    std::unordered_map<int, CS> CSPackage;
    std::unordered_map<int, std::unordered_map<int, int>> graph;
    std::vector<int> suitableDiameters = { 400, 500, 700, 1000, 1400 };

public:
    GTN();

    int FindOrCreatePipe(int diameter);

    bool ConnectCS(int startCSID, int endCSID, int pipeID);
    bool isSuitable(int diameter) const;

    std::vector<int> TopologicalSort() const;

    void DisplayNetwork() const;

    std::unordered_map<int, Pipe>& GetPipePackage();
    std::unordered_map<int, CS>& GetCSPackage();
    std::unordered_map<int, std::unordered_map<int, int>>& getGraphMat();

    void DisplayPipes() const;
    void DisplayCompressorStations() const;
    void DeleteGTN();
};