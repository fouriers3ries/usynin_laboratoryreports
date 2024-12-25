#pragma once
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"
#include "CS.h"
#include <algorithm>

#define INPUT_LINE(in, string) std::getline(in >> std::ws, string); \
						std::cerr << string << '\n';

class redirect_output_wrapper
{
	std::ostream& stream;
	std::streambuf* const old_buf;
public:
	redirect_output_wrapper(std::ostream& src)
		:old_buf(src.rdbuf()), stream(src)
	{
	}
	~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
	}
	void redirect(std::ostream& dest)
	{
		stream.rdbuf(dest.rdbuf());
	}
};

bool CheckPipeByName(const Pipe& pipeToCheck, const std::string arg);
bool CheckPipeByStatus(const Pipe& pipeToCheck, bool arg);
bool CheckCSByName(const CS& CSToCheck, const std::string arg);
bool CheckCSByAFK(const CS& CSToCheck, int arg);

void PipePackageEdit(std::unordered_map<int, Pipe>& pipePackage, std::unordered_set<int> res);
void CSPackageEdit(std::unordered_map<int, CS>& CSPackage, std::unordered_set <int>& res);

template<typename T>
using PipeFilter = bool(*)(const Pipe& pipePackage, T arg);

template<typename T>
using CSFilter = bool(*)(const CS& CSPackage, T arg);

template<typename T>
std::unordered_set<int> FindPipeByFilter(const std::unordered_map<int, Pipe>& pipePackage, PipeFilter<T> filter, T arg) {
	std::unordered_set<int> output;
	for (auto& pipe : pipePackage) {
		if (filter(pipe.second, arg)) {
			output.emplace(pipe.first);
			std::cout << pipe.second;
		}
	}
	return output;
}

template<typename T>
std::unordered_set<int> FindCSByFilter(const std::unordered_map<int, CS>& CSPackage, CSFilter<T> filter, T arg) {
	std::unordered_set<int> output;
	for (auto& CS : CSPackage) {
		if (filter(CS.second, arg)) {
			output.emplace(CS.first);
			std::cout << CS.second;
		}
	}
	return output;
}

template<typename T>
void ObjectPackageDelete(std::unordered_map<int, T>& objectPackage,
    std::unordered_set<int> res,
    std::unordered_map<int, std::unordered_map<int, int>>& graph,
    bool isPipe = false) {
    std::string IDPackage;
    std::cout << "Enter IDs of the objects you want to delete: \n";
    std::getline(std::cin >> std::ws, IDPackage);
    std::istringstream IDStream(IDPackage); // package of IDs -> stringstream object to parse those IDs
    std::unordered_set<int> IDs;
    int ID;

    while (IDStream >> ID) {
        IDs.emplace(ID);
    }

    for (int ID : IDs) {
        auto obj = objectPackage.find(ID);

        if (res.find(ID) == res.end()) {
            std::cout << "There's no object with ID: " << ID << '\n';
            continue;
        }

        bool inGraph = false;

        if (isPipe) {
            for (const auto& [startCSID, edges] : graph) {
                if (std::any_of(edges.begin(), edges.end(), [&](const auto& edge) { return edge.second == ID; })) {
                    inGraph = true;
                    break;
                }
            }
        }
        else {
            if (graph.find(ID) != graph.end()) {
                inGraph = true;
            }
            for (const auto& [startCSID, edges] : graph) {
                if (edges.find(ID) != edges.end()) {
                    inGraph = true;
                    break;
                }
            }
        }

        if (inGraph) {
            std::cout << "Object with ID " << ID << " is used in the graph. Are you sure you want to delete it? (yes/no): ";
            std::string userResponse;
            std::cin >> userResponse;
            if (userResponse != "yes") {
                std::cout << "Object with ID " << ID << " was not deleted.\n";
                continue;
            }
        }

        if (isPipe) {
            for (auto& [startCSID, edges] : graph) {
                for (auto it = edges.begin(); it != edges.end();) {
                    if (it->second == ID) {
                        it = edges.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }

        else {
            graph.erase(ID);
            for (auto& [startCSID, edges] : graph) {
                edges.erase(ID);
            }
        }

        if (obj != objectPackage.end()) {
            objectPackage.erase(obj);
            std::cout << "Object with ID " << ID << " was successfully deleted.\n";
        }
    }
}

template <typename T>
T GetCorrectInput(T min, T max) {
	T value;
	while ((std::cin >> value).fail() || value < min || value > max) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "\nError. Input must be a value between " << min << " and " << max << '\n';
		std::cout << "Try again: ";
	}
	std::cerr << value << '\n';
	return value;
};

