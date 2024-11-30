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

/*
void Save(const Pipe& pipeToSave, const CS& CSToSave) {
	std::ofstream file("data.txt");
	if (file.is_open()) {
		if (pipeToSave.pipeName != "") {
			file << "1\n";
			SavePipe(pipeToSave, file);
		}
		else {
			std::cout << "Pipe data was not saved! (there's no created pipe)\n";
			file << "0\n";
		}
		if (CSToSave.CSName != "") {
			file << "1\n";
			SaveCS(CSToSave, file);
		}
		else {
			std::cout << "CS data was not saved! (there's no created CS)\n";
			file << "0\n";
		}
		file.close();
	}
	else std::cout << "File cannot be open!\n";

}
*/

/*
void LoadPipe(Pipe& pipeToEdit, std::ifstream& file) {
	std::getline(file >> std::ws, pipeToEdit.pipeName);
	file >> pipeToEdit.length >> pipeToEdit.diameter >> pipeToEdit.isRepairing;
}
*/

/*
void Load(Pipe& pipeToEdit, CS& CSToEdit) {
	std::ifstream file("data.txt");
	int pipesQuantity = 0;
	int CSQuantity = 0;
	if (file.is_open()) {
		file >> pipesQuantity;
		if (pipesQuantity) for (size_t i = 0; i < pipesQuantity; ++i) LoadPipe(pipeToEdit, file);
		else pipeToEdit.length = 0;
		file >> CSQuantity;
		if (CSQuantity) for (size_t i = 0; i < CSQuantity; ++i) LoadCS(CSToEdit, file);
		else CSToEdit.workshopsQuantity = 0;
		file.close();
	}
	else std::cout << "File cannot be open!\n";
}
*/

int main() {
	std::unordered_map<int, Pipe> pipePackage;
	std::unordered_map<int, CS> CSPackage;
	std::unordered_set<int> output;

	std::string searchingName{};
	bool searchingStatus{};
	int searchingPercentage{};
	std::unordered_set<int> searchResult{};
	int userChoice{};

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
		case 5:
			break;
		case 6:
			break;
		case 0:
			return 0;
		}

	}
}