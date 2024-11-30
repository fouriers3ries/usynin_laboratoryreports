#include <iostream>
#include <fstream>
#include <string>
#include "Pipe.h"
#include "CS.h"
#include "utilities.h"
#include <unordered_map>

void ShowMenu() {
	std::cout << "Second laboratory work report by Usynin Daniil (AS-23-04)\n\n" <<
		"Choose an option to do:\n" << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Search for object\n" << "5. Save\n" << "6. Load\n" << "0. Exit\n\n";
};

void ShowSearchMenu() {
	std::cout << "1. Search for pipe by name\n" << "2. Search for pipe by repairing status\n" <<
		"3. Search for CS by name\n" << "4. Search for CS by workshops\n" << "0. Exit to main menu\n";
}

void ShowPurposeMenu() {
	std::cout << "1. Edit\n" << "2. Delete\n" << "0. Back\n";
}

template<typename T>
void SaveObject(std::ofstream& path, std::unordered_map<int, T>& objectPackage) {
	for (auto& object : objectPackage) {
		path << object.second;
	}
}

template<typename T>
std::unordered_map<int, T> LoadObject(std::ifstream& inputFile, std::unordered_map<int, T>& objectPackage, int quantity) {
	for (size_t i = 0; i < quantity; i++) {
		T object;
		inputFile >> object;
		objectPackage.emplace(object.GetID(), object);
	}
	return objectPackage;
}

void Load(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage) {
	std::ifstream inputFile;
	std::string path;

	std::cout << "Enter file name: \n";
	std::getline(std::cin >> std::ws, path);
	inputFile.open(path);

	if (inputFile.is_open()) {
		int pipesQuantity, CSQuantity;
		inputFile >> pipesQuantity;
		LoadObject(inputFile, pipePackage, pipesQuantity);
		inputFile >> CSQuantity;
		LoadObject(inputFile, CSPackage, CSQuantity);
		std::cout << "Data loaded successfully\n";
	} else std::cout << "There're some problems during reading from file. Try again.\n";
	inputFile.close();
}

int main() {
	std::unordered_map<int, Pipe> pipePackage;
	std::unordered_map<int, CS> CSPackage;
	std::unordered_set<int> output;

	std::string searchingName{};
	bool searchingStatus{};
	int searchingPercentage{};
	std::unordered_set<int> searchResult{};
	int userChoice{};
	std::string path{};

	for (;;) {
		ShowMenu();
		switch (GetCorrectInput(0, 6)) {
		case 1: {
			Pipe pipeToAdd;
			std::cin >> pipeToAdd;
			pipePackage.emplace(pipeToAdd.GetID(), pipeToAdd);
			break;
		}
		case 2: {
			CS CSToAdd;
			std::cin >> CSToAdd;
			CSPackage.emplace(CSToAdd.GetID(), CSToAdd);
			break;
		}
		case 3: {
			if (pipePackage.empty()) std::cout << "There's no added pipes!\n";
			else for (const auto& pipe : pipePackage) std::cout << pipe.second << '\n';
			if (CSPackage.empty()) std::cout << "There's no added CS's!\n";
			else for (const auto& CS : CSPackage) std::cout << CS.second << '\n';
			break;
		}
		case 4: {
			bool exitSearchMenu{ false };
			while (!exitSearchMenu) {
				ShowSearchMenu();
				switch (GetCorrectInput(0, 4)) {
				case 1:
					if (pipePackage.empty()) std::cout << "There's no added pipes\n";
					else {
						std::cout << "Enter name for searching: \n";
						std::getline(std::cin >> std::ws, searchingName);
						searchResult = FindPipeByFilter(pipePackage, CheckPipeByName, searchingName);
						if (!searchResult.empty()) {
							ShowPurposeMenu(); // check if users want to edit or delete those pipes
							userChoice = GetCorrectInput(0, 2);
							if (userChoice == 1) PipePackageEdit(pipePackage, searchResult);
							if (userChoice == 2) ObjectPackageDelete(pipePackage, searchResult);
							if (userChoice == 0) break;
						}
						else std::cout << "There's no pipes with that name!" << '\n';
					}
					break;
				case 2:
					if (pipePackage.empty()) std::cout << "There's no added pipes\n";
					else {
						std::cout << "Enter pipe repairing status for searching (1 - not in repairing, 0 - in repairing): \n";
						searchingStatus = GetCorrectInput(0, 1);
						searchResult = FindPipeByFilter(pipePackage, CheckPipeByStatus, searchingStatus);
						if (!searchResult.empty()) {
							ShowPurposeMenu();
							userChoice = GetCorrectInput(0, 2);
							if (userChoice == 1) PipePackageEdit(pipePackage, searchResult);
							if (userChoice == 2) ObjectPackageDelete(pipePackage, searchResult);
							if (userChoice == 0) break;
						}
						else std::cout << "There's no pipes with that repairing status!" << '\n';
					}
					break;
				case 3:
					if (CSPackage.empty()) std::cout << "There's no added CS's\n";
					else {
						std::cout << "Enter name for searching: \n";
						std::getline(std::cin >> std::ws, searchingName);
						searchResult = FindCSByFilter(CSPackage, CheckCSByName, searchingName);
						if (!searchResult.empty()) {
							ShowPurposeMenu();
							userChoice = GetCorrectInput(0, 2);
							if (userChoice == 1) CSPackageEdit(CSPackage, searchResult);
							if (userChoice == 2) ObjectPackageDelete(CSPackage, searchResult);
							if (userChoice == 0) break;
						}
						else std::cout << "There's no CS's with that name!\n";
					}
					break;
				case 4:
					if (CSPackage.empty()) std::cout << "There's no added CS's\n";
					else {
						std::cout << "Enter disability percentage for searching: \n";
						searchingPercentage = GetCorrectInput(0, 100);
						searchResult = FindCSByFilter(CSPackage, CheckCSByAFK, searchingPercentage);
						if (!searchResult.empty()) {
							ShowPurposeMenu();
							userChoice = GetCorrectInput(0, 2);
							if (userChoice == 1) CSPackageEdit(CSPackage, searchResult);
							if (userChoice == 2) ObjectPackageDelete(CSPackage, searchResult);
							if (userChoice == 0) break;
						}
						else std::cout << "There's no CS's with that disability percentage!\n";
					}
					break;
				case 0:
					exitSearchMenu = !exitSearchMenu;
					break;
				}
			}
			break;
		}
		case 5: {
			std::ofstream outputFile;
			std::cout << "Enter file name: \n";
			std::getline(std::cin >> std::ws, path);
			outputFile.open(path + ".txt", std::ios::app);
			if (outputFile.is_open()) {
				outputFile << pipePackage.size() << '\n';
				SaveObject(outputFile, pipePackage);
				outputFile << CSPackage.size() << '\n';
				SaveObject(outputFile, CSPackage);
				std::cout << "Data writed to file successfully\n";
			}
			else std::cout << "Failed! Data cannot be written.";
			outputFile.close();
		}
			break;
		case 6: Load(pipePackage, CSPackage);
			break;
		case 0:
			return 0;
		}

	}
}