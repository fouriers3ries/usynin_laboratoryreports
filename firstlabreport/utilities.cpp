#include "utilities.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"
#include "CS.h"

bool CheckPipeByName(const Pipe& pipeToCheck, const std::string arg) {
	return pipeToCheck.GetName().find(arg) != std::string::npos;
}

bool CheckPipeByStatus(const Pipe& pipeToCheck, bool arg) {
	return pipeToCheck.GetRepairingStatus() == arg;
}

bool CheckCSByName(const CS& CSToCheck, const std::string arg) {
	return CSToCheck.GetName().find(arg) != std::string::npos;
}

bool CheckCSByAFK(const CS& CSToCheck, int arg) {
	return CSToCheck.GetAFK() >= arg;
}

void PipePackageEdit(std::unordered_map<int, Pipe>& pipePackage, std::unordered_set<int> res) {
	std::string IDPackage;
	std::cout << "Enter ID's of those pipes you wanna edit: \n";
	std::getline(std::cin >> std::ws, IDPackage);
	std::istringstream IDStream(IDPackage); // package of ID's -> stringstream object to parse those ID's
	std::unordered_set<int> IDs;
	int ID;

	while (IDStream >> ID) {
		IDs.emplace(ID);
	}

	for (int ID : IDs) {
		if (res.find(ID) == res.end()) {
			std::cout << "There's no pipe with ID: " << ID << '\n';
			continue;
		}
		pipePackage.at(ID).EditPipe();
		std::cout << "Pipe with ID " << ID << " successfully edited\n";
	}
}

void CSPackageEdit(std::unordered_map<int, CS>& CSPackage, std::unordered_set <int>& res) {
	std::string IDPackage;
	std::cout << "Enter ID's of those CS's you wanna edit: \n";
	std::getline(std::cin >> std::ws, IDPackage);
	std::istringstream IDStream(IDPackage);
	std::unordered_set<int> IDs;
	int ID;

	while (IDStream >> ID) {
		IDs.emplace(ID);
	}

	for (int ID : IDs) {
		if (res.find(ID) == res.end()) {
			std::cout << "There's no CS with ID: " << ID << '\n';
			continue;
		}
		CSPackage.at(ID).EditCS();
		std::cout << "CS with ID " << ID << " successfully edited\n";
	}
}