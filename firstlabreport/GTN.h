#pragma once
#include <unordered_map>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "CS.h"

class GTN {
private:
    std::unordered_map<int, Pipe> pipePackage; // Трубы
    std::unordered_map<int, CS> CSPackage; // КС
    std::unordered_map<int, std::unordered_map<int, int>> graph; // Динамическая структура графа (диаметр трубы между КС)

    // Вспомогательная функция для поиска или создания трубы
    int FindOrCreatePipe(int diameter);

public:
    GTN();

    // Специфические методы для добавления труб и КС
    void AddPipe();
    void AddCompressorStation();
    void CreateSuitablePipe(int diameter);

    // Соединение КС трубой
    bool ConnectCS(int startCSID, int endCSID, int diameter);

    // Топологическая сортировка
    std::vector<int> TopologicalSort() const;

    // Вывод информации о графе
    void DisplayNetwork() const;

    std::unordered_map<int, Pipe> GetPipePackage() const;
    std::unordered_map<int, CS> GetCSPackage() const;

    // Вывод информации о трубах и КС
    void DisplayPipes() const;
    void DisplayCompressorStations() const;
};