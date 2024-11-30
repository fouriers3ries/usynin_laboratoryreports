#pragma once
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"
#include "CS.h"

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

void PipePackageEdit(std::unordered_map<int, Pipe>& pipePackage, std::unordered_set<int> res);
void CSPackageEdit(std::unordered_map<int, CS>& CSPackage, std::unordered_set <int>& res);

template<typename T>
void ObjectPackageDelete(std::unordered_map<int, T>& objectPackage, std::unordered_set<int> res) {
	std::string IDPackage;
	std::cout << "Enter ID's of those objects you wanna delete: \n";
	std::getline(std::cin >> std::ws, IDPackage);
	std::istringstream IDStream(IDPackage); // package of ID's -> stringstream object to parse those ID's
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
		} if (obj != objectPackage.end()) {
			objectPackage.erase(obj);
			std::cout << "Object with ID " << ID << " was successfully deleted\n";
		}
	}

}

template <typename T>
T GetCorrectInput(T min, T max) {
	T value;
	while ((std::cin >> value).fail() || value < min || value > max) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "\nfuck you. input must be a value between " << min << " and " << max << '\n';
		std::cout << "enter your fucking value: ";
	}
	return value;
};

