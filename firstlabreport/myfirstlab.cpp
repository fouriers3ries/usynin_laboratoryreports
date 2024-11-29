#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Pipe.h"
#include <unordered_map>

struct CS {
	std::string CSName = "";
	int workshopsQuantity = 0;
	int busyWorkshopsQuantity = 0;
	float effectiveness = 0.0f;
};

void ShowMenu() {
	std::cout << "Second laboratory work report by Usynin Daniil (AS-23-04)\n\n" <<
		"Choose an option to do:\n" << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Edit pipe\n" << "5. Edit CS\n"
		<< "6. Save\n" << "7. Load\n" << "0. Exit\n\n";
};

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

void LoadCS(CS& CSToEdit, std::ifstream& file) {
	std::getline(file >> std::ws, CSToEdit.CSName);
	file >> CSToEdit.workshopsQuantity >> CSToEdit.busyWorkshopsQuantity >>
		CSToEdit.effectiveness;
}

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
	int userChoice;
	std::unordered_map<Pipe, int> pipes = {};
	Pipe pipe = {};
	CS CS = {};
	for (;;) {
		ShowMenu();
		std::cin >> userChoice;
		while (std::cin.fail() || !isUserChoiceCorrect(userChoice)) {
			ProcessError(0);
			std::cin >> userChoice;
		}
		switch (userChoice) {
		case 1:
			std::cin >> pipe;
			
			break;
		case 2:
			std::cin >> CS;
			break;
		case 3:
			std::cout << pipe;
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else std::cout << CS;
			break;
		case 4:
			/*
			if (pipe.length == 0) std::cout << "There's no added pipe\n";
			else pipe.EditPipe();
			*/
			break;
		case 5:
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else EditCS(CS);
			break;
		case 6:
			break;
		case 7:
			break;
		case 0:
			return 0;
		}

	}
}