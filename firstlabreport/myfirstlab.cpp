#include <iostream>
#include <fstream>
#include <string>
#include "Pipe.h"
#include "CS.h"
#include "utilities.h"
#include <unordered_map>
#include <chrono>
// #include <format>

void ShowMenu() {
	std::cout << "Second laboratory work report by Usynin Daniil (AS-23-04)\n\n" <<
		"Choose an option to do:\n" << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Search for object\n" << "5. Save\n" << "6. Load\n" << "0. Exit\n\n";
};

void ShowSearchMenu() {
	std::cout << "1. Search for pipe by name\n" << "2. Search for pipe by repairing status\n" <<
		"3. Search for CS by name\n" << "4. Search for CS by workshops\n" << "0. Exit to main menu\n";
}

void ShowQuantityMenu() {
	std::cout << "1. Perform action by ID\n" << "2. Perform action with all objects\n" << "0. Back\n";
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
	inputFile.open(path + ".txt.");

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

void Save(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS> CSPackage) {
	std::string path{};
	std::ofstream outputFile{};
	std::cout << "Enter file name: \n";
	INPUT_LINE(std::cin, path);
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

void EditPipesByChoice(std::unordered_map<int, Pipe>& pipePackage, std::unordered_set<int>& searchResult, int userChoice) {
	if (userChoice == 1) PipePackageEdit(pipePackage, searchResult);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			pipePackage.at(ID).EditPipe();
		}
		std::cout << "All suitable pipes was successfully edited\n";
	}
}

void EditCSByChoice(std::unordered_map<int, CS>& CSPackage, std::unordered_set<int>& searchResult, int userChoice) {
	if (userChoice == 1) CSPackageEdit(CSPackage, searchResult);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			CSPackage.at(ID).EditCS();
		}
		std::cout << "All suitable CS's was successfully edited\n";
	}
}

template<typename T>
void DeleteObjectsByChoice(std::unordered_map<int, T>& objectPackage, std::unordered_set<int>& searchResult, int userChoice) {
	if (userChoice == 1) ObjectPackageDelete(objectPackage, searchResult);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			auto obj = objectPackage.find(ID);
			objectPackage.erase(obj);
		}
		std::cout << "All suitable objects was successfully deleted\n";
	}
}

void ShowObjects(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage) {
	if (pipePackage.empty())
		std::cout << "There's no added pipes!\n";
	else
		for (const auto& pipe : pipePackage)
			std::cout << pipe.second << '\n';
	if (CSPackage.empty())
		std::cout << "There's no added CS's!\n";
	else
		for (const auto& CS : CSPackage)
			std::cout << CS.second << '\n';
}

template<typename T>
void AddObject(std::unordered_map<int, T> objectPackage) {
	T objToAdd;
	std::cin >> objToAdd;
	objectPackage.emplace(objToAdd.GetID(), objToAdd);
}

void SearchPipesByName(std::unordered_map<int, Pipe>& pipePackage) {
	std::string searchingName{};
	std::cout << "Enter name for searching: \n";
	INPUT_LINE(std::cin, searchingName);
	auto searchResult = FindPipeByFilter(pipePackage, CheckPipeByName, searchingName);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditPipesByChoice(pipePackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 0) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(pipePackage, searchResult, userChoiceForQuantity);
		}
	}
	else std::cout << "There's no pipes with that name!" << '\n';
}

void SearchPipesByStatus(std::unordered_map<int, Pipe>& pipePackage) {
	bool searchingStatus{};
	std::cout << "Enter pipe repairing status for searching (1 - not in repairing, 0 - in repairing): \n";
	searchingStatus = GetCorrectInput(0, 1);
	auto searchResult = FindPipeByFilter(pipePackage, CheckPipeByStatus, searchingStatus);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditPipesByChoice(pipePackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 0) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(pipePackage, searchResult, userChoiceForQuantity);
		}
	}
	else std::cout << "There's no pipes with that status!\n";
}

void SearchCSByName(std::unordered_map<int, CS>& CSPackage) {
	std::string searchingName{};
	std::cout << "Enter name for searching: \n";
	INPUT_LINE(std::cin, searchingName);
	auto searchResult = FindCSByFilter(CSPackage, CheckCSByName, searchingName);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditCSByChoice(CSPackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 3) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(CSPackage, searchResult, userChoiceForQuantity);
		}
	}
	else std::cout << "There's no CS's with that name!\n";
}

void SearchCSByStatus(std::unordered_map<int, CS>& CSPackage) {
	int searchingPercentage{};
	std::cout << "Enter disability percentage for searching: \n";
	searchingPercentage = GetCorrectInput(0, 100);
	auto searchResult = FindCSByFilter(CSPackage, CheckCSByAFK, searchingPercentage);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditCSByChoice(CSPackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 3) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(CSPackage, searchResult, userChoiceForQuantity);
		}
	}
	else std::cout << "There's no CS's with that disability percentage!\n";
}

void MainMenu() {
	std::unordered_map<int, Pipe> pipePackage;
	std::unordered_map<int, CS> CSPackage;
	std::unordered_set<int> output;

	while(1) {
		ShowMenu();
		switch (GetCorrectInput(0, 6)) {
		case 1: {
			AddObject(pipePackage);
			break;
		}
		case 2: {
			AddObject(CSPackage);
			break;
		}
		case 3: {
			ShowObjects(pipePackage, CSPackage);
			break;
		}
		case 4: {
			bool exitSearchMenu{ false };
			while (!exitSearchMenu) {
				ShowSearchMenu();
				switch (GetCorrectInput(0, 4)) {
				case 1:
					if (pipePackage.empty()) std::cout << "There's no added pipes\n";
					else SearchPipesByName(pipePackage);
					break;
				case 2:
					if (pipePackage.empty()) std::cout << "There's no added pipes\n";
					else SearchPipesByStatus(pipePackage);
					break;
				case 3:
					if (CSPackage.empty()) std::cout << "There's no added CS's\n";
					else SearchCSByName(CSPackage);
					break;
				case 4:
					if (CSPackage.empty()) std::cout << "There's no added CS's\n";
					else SearchCSByStatus(CSPackage);
					break;
				case 0:
					exitSearchMenu = !exitSearchMenu;
					break;
				}
			}
			break;
		}
		case 5: Save(pipePackage, CSPackage);
			break;
		case 6: Load(pipePackage, CSPackage);
			break;
		case 0:
			return;
		}
	}
}

int main() {

	redirect_output_wrapper cerr_out(std::cerr);
	std::string time = "fdsfdsf";
	std::ofstream logfile("log_" + time + ".txt");
	if (logfile)
		cerr_out.redirect(logfile);

	MainMenu();

}