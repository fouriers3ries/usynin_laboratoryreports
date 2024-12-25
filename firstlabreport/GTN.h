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
    std::unordered_map<int, std::unordered_map<int, double>> capacities;
    std::vector<int> suitableDiameters = { 400, 500, 700, 1000, 1400 };
    std::vector<int> all_nodes;

public:
    GTN();

    bool ConnectCS(int startCSID, int endCSID, int pipeID);
    bool isSuitable(int diameter) const;

    std::vector<int> TopologicalSort() const;

    void DisplayNetwork() const;

    std::unordered_map<int, Pipe>& GetPipePackage();
    std::unordered_map<int, CS>& GetCSPackage();
    std::unordered_map<int, std::unordered_map<int, int>>& getGraphMat();

    // Функции для нахождения максимального потока методом Форда-Фалкерсона
    float EdmondsKarp(int source, int sink);
    bool BFS(int source, int sink, std::unordered_map<int, int>& parent);

    // Функция для нахождения кратчайшего пути между двумя КС
    std::vector<int> FindShortestPath(int startCSID, int endCSID);

    double calculatePipeCapacity(int pipeID) const;

    void DeleteGTN();
};