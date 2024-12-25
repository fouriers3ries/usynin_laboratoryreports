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
    all_nodes.push_back(startCSID);
    all_nodes.push_back(endCSID);
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

bool GTN::BFS(int source, int sink, std::unordered_map<int, int>& parent) {
    std::unordered_map<int, bool> visited;
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto& [v, _] : graph[u]) {
            // Используем метод GetCapacity для получения пропускной способности
            double cap = pipePackage.at(v).GetPerformance();
            if (!visited[v] && cap > 0) {
                q.push(v);
                visited[v] = true;
                parent[v] = u;
                if (v == sink) return true;
            }
        }
    }
    return false;
}

float GTN::EdmondsKarp(int source, int sink) {
    std::unordered_map<int, int> parent;
    float maxFlow = 0.0;

    auto BFS = [&](int s, int t) -> bool {
        std::unordered_map<int, bool> visited;
        std::queue<int> queue;

        queue.push(s);
        visited[s] = true;
        parent.clear();

        while (!queue.empty()) {
            int current = queue.front();
            queue.pop();

            for (const auto& [adjNode, pipeID] : graph[current]) {
                if (!visited[adjNode] && pipePackage.at(pipeID).GetPerformance() > 0) {
                    parent[adjNode] = current;
                    if (adjNode == t) {
                        return true;
                    }
                    visited[adjNode] = true;
                    queue.push(adjNode);
                }
            }
        }
        return false;
        };

    while (BFS(source, sink)) {
        float pathFlow = std::numeric_limits<double>::infinity();

        // Найти минимальную производительность на пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            int pipeID = graph[u][v];
            pathFlow = std::min(pathFlow, pipePackage.at(pipeID).GetPerformance());
        }

        // Обновить значения производительности на пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            int forwardPipeID = graph[u][v];
            int reversePipeID = graph[v].count(u) > 0 ? graph[v][u] : -1;

            // Уменьшить производительность прямого ребра
            double currentPerformance = pipePackage.at(forwardPipeID).GetPerformance();
            pipePackage.at(forwardPipeID).SetPerformance(currentPerformance - pathFlow);

            // Увеличить производительность обратного ребра
            if (reversePipeID == -1) { // Проверяем, есть ли обратное ребро
                Pipe reversePipe;
                reversePipe.CreateReversePipe(pipePackage, forwardPipeID, pipePackage.at(forwardPipeID));
                graph[v][u] = reversePipe.GetID(); // Добавляем обратное ребро в граф
            }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

std::vector<int> GTN::FindShortestPath(int start, int end) {
    std::unordered_map<int, double> distances;
    std::unordered_map<int, int> previous;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;

    // Заполняем веса рёбер на основе данных из pipePackage
    for (auto& [from, edges] : graph) {
        for (auto& [to, pipeID] : edges) {
            if (pipePackage.contains(pipeID)) {
                Pipe& pipe = pipePackage[pipeID];
                // Если труба требует ремонта, вес будет бесконечным
                double weight = pipe.GetRepairingStatus() ? std::numeric_limits<double>::infinity() : pipe.GetLength();
                edges[to] = weight;  // Обновляем вес рёбер
            }
        }
    }

    // Инициализация расстояний и предыдущих узлов
    for (int node : all_nodes) {
        distances[node] = std::numeric_limits<double>::infinity();
        previous[node] = -1;
    }
    distances[start] = 0;

    // Помещаем начальный узел в очередь
    pq.push({ 0.0, start });

    while (!pq.empty()) {
        std::pair<double, int> current = pq.top();
        pq.pop();

        double currentDistance = current.first;
        int currentNode = current.second;

        // Если это расстояние неактуально, пропускаем
        if (currentDistance > distances[currentNode]) {
            continue;
        }

        // Обрабатываем соседей текущего узла
        for (const auto& [neighbor, pipeID] : graph[currentNode]) {
            double newDistance = currentDistance + pipePackage[pipeID].GetLength();  // Добавляем длину трубы

            if (newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                previous[neighbor] = currentNode;
                pq.push({ newDistance, neighbor });
            }
        }
    }

    // Построение пути
    std::vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    // Проверяем, существует ли путь
    if (path.size() == 1 && path[0] != start) {
        return {};  // Путь не найден
    }

    return path;
}