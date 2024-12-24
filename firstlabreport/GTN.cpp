#include "GTN.h"
#include <queue>
#include <algorithm>

GTN::GTN() {}

int GTN::FindOrCreatePipe(int diameter) {
    // Ищем существующую трубу с подходящим диаметром, которая не в ремонте
    for (const auto& [id, pipe] : pipePackage) {
        if (pipe.GetDiameter() == diameter && !pipe.GetRepairingStatus()) {
            return id;
        }
    }

    // Если подходящей трубы нет, создаем новую
    Pipe newPipe;
    std::cout << "No available pipes with diameter " << diameter << " mm. Creating a new pipe.\n";
    std::cin >> newPipe;

    // Вывод предупреждения, если введенный диаметр не совпадает с запрошенным
    if (newPipe.GetDiameter() != diameter) {
        std::cout << "Warning: Entered pipe diameter does not match the requested diameter. Proceeding with the entered value.\n";
    }

    // Добавляем новую трубу в pipePackage
    pipePackage.emplace(newPipe.GetID(), newPipe);

    // Возвращаем ID новой трубы
    return newPipe.GetID();
}

bool GTN::ConnectCS(int startCSID, int endCSID, int diameter) {
    // Проверяем, существуют ли такие КС
    if (CSPackage.find(startCSID) == CSPackage.end() ||
        CSPackage.find(endCSID) == CSPackage.end()) {
        std::cout << "Invalid CS IDs.\n";
        return false;
    }

    // Ищем свободную трубу с заданным диаметром
    int pipeID = -1; // ID выбранной трубы
    for (const auto& [id, pipe] : pipePackage) {
        if (pipe.GetRepairingStatus() == false && pipe.GetDiameter() == diameter) {
            pipeID = id;
            break;
        }
    }

    // Если свободной трубы нет, создаем новую
    if (pipeID == -1) {
        Pipe newPipe;
        std::cout << "No available pipes with diameter " << diameter << " mm. Creating a new pipe.\n";
        std::cin >> newPipe;
        if (newPipe.GetDiameter() != diameter) {
            std::cout << "Warning: Entered pipe diameter does not match the requested diameter. Proceeding with the entered value.\n";
        }
        pipePackage.emplace(newPipe.GetID(), newPipe);
        pipeID = newPipe.GetID();
    }

    // Добавляем соединение в граф
    graph[startCSID][endCSID] = pipeID;
    return true;
}

// Топологическая сортировка
std::vector<int> GTN::TopologicalSort() const {
    std::unordered_map<int, int> inDegree;
    std::vector<int> sortedNodes;

    // Инициализация степеней входа
    for (const auto& [node, edges] : graph) {
        if (inDegree.find(node) == inDegree.end()) {
            inDegree[node] = 0;
        }
        for (const auto& [adjNode, _] : edges) {
            inDegree[adjNode]++;
        }
    }

    // Поиск вершин с inDegree = 0
    std::vector<int> zeroInDegree;
    for (const auto& [node, degree] : inDegree) {
        if (degree == 0) {
            zeroInDegree.push_back(node);
        }
    }

    // Основной алгоритм
    while (!zeroInDegree.empty()) {
        int node = zeroInDegree.back();
        zeroInDegree.pop_back();
        sortedNodes.push_back(node);

        for (const auto& [adjNode, _] : graph.at(node)) {
            inDegree[adjNode]--;
            if (inDegree[adjNode] == 0) {
                zeroInDegree.push_back(adjNode);
            }
        }
    }

    // Проверка на циклы
    if (sortedNodes.size() != graph.size()) {
        std::cout << "Cycle detected in the graph.\n";
        return {};
    }

    return sortedNodes;
}

std::unordered_map<int, Pipe> GTN::GetPipePackage() const {
    return pipePackage;
}

std::unordered_map<int, CS> GTN::GetCSPackage() const {
    return CSPackage;
}


// Вывод информации о графе
void GTN::DisplayNetwork() const {
    for (const auto& [startCSID, edges] : graph) {
        for (const auto& [endCSID, pipeID] : edges) {
            std::cout << "CS " << startCSID << " -> CS " << endCSID
                << " via Pipe ID " << pipeID << '\n';
        }
    }
}