#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// structure defining pipe properties
struct Pipe {
	std::string pipeName;
	int length = 0;
	float diameter = 0.0f;
	std::string isRepairing;
	bool isCreated = false;
};

// structure defining CS properties
struct CS {
	std::string CSName;
	int workshopsQuantity = 0;
	float effectiveness = 0.0f;
	bool isCreated = false;
};


void CreateMenu() {
	std::cout << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Edit pipe\n" <<
		"5. Edit CS\n" << "6. Save\n" << "7. Load\n" <<
		"0. Quit\n\n";
};

// function for new pipe adding & editing
void EditPipe(Pipe& pipeToEdit) {
	;
	pipeToEdit.isCreated = true;
	std::cout << "Enter pipe name: \n";
	std::cin.ignore(32767, '\n');
	std::getline(std::cin, pipeToEdit.pipeName);
	std::cout << "Enter pipe length: \n";
	std::cin >> pipeToEdit.length;
	std::cout << "Enter pipe diameter: \n";
	std::cin >> pipeToEdit.diameter;
	std::cout << "Enter pipe repairing status (0 - in repairing, 1 - not in repairing): \n";
	int status;
	std::cin >> status;
	status == 0 ? pipeToEdit.isRepairing = "in repairing" : pipeToEdit.isRepairing = "not in repairing";
};

// function for new compression station adding & editing
void EditCS(CS& CSToEdit) {
	CSToEdit.isCreated = true;
	std::cout << "Enter CS name: \n";
	std::cin >> CSToEdit.CSName;
	std::cout << "Enter workshops quantity: \n";
	std::cin >> CSToEdit.workshopsQuantity;
	std::cout << "Enter effectiveness: \n";
	std::cin >> CSToEdit.effectiveness;
}

// function for all objects output
void ShowObjects(Pipe& pipeToShow, CS& CSToShow) {
	!pipeToShow.isCreated ? std::cout << "There's no added pipes\n" : std::cout << "-- Pipe properties -- \n" <<
		"Pipe name: " << pipeToShow.pipeName << '\n' <<
		"Pipe length: " << pipeToShow.length << '\n' <<
		"Pipe diameter: " << pipeToShow.diameter << '\n' <<
		"Pipe repairing status: " << "in repairing\n\n";
	!CSToShow.isCreated ? std::cout << "There's no added CS's\n\n" : std::cout << "-- CS properties -- \n" <<
		"CS name: " << CSToShow.CSName << '\n' <<
		"CS workshops quantity: " << CSToShow.workshopsQuantity << '\n' <<
		"CS effectiveness: " << CSToShow.effectiveness << '\n' << '\n';
}

void Save(std::string filepath, Pipe& pipeToWrite, CS& CSToWrite) {
	std::ofstream fout(filepath, std::ios::out);
	!pipeToWrite.isCreated ? fout << "There's no added pipes\n" : fout << "Pipe name: " << pipeToWrite.pipeName << '\n' <<
		"Pipe length: " << pipeToWrite.length << '\n' <<
		"Pipe diameter: " << pipeToWrite.diameter << '\n' <<
		"Pipe repairing status: " << pipeToWrite.isRepairing << '\n' << '\n';
	!CSToWrite.isCreated ? fout << "There's no added CS's\n" : fout << "CS name: " << CSToWrite.CSName << '\n' <<
		"CS workshops quantity: " << CSToWrite.workshopsQuantity << '\n' <<
		"CS effectiveness: " << CSToWrite.effectiveness;
	fout.close();
}

int main() {
	int userChoice;
	Pipe newPipe;
	Pipe& pipeToEdit = newPipe;
	CS newCS;
	CS& CSToEdit = newCS;
	std::string filepath;
	while (std::cin) {
		CreateMenu();
		std::cin >> userChoice;
		switch (userChoice) {
		case 1:
			EditPipe(pipeToEdit);
			break;
		case 2:
			EditCS(CSToEdit);
			break;
		case 3:
			ShowObjects(pipeToEdit, CSToEdit);
			break;
		case 4:
			!newPipe.isCreated ? throw "There's no added pipes\n" : EditPipe(pipeToEdit);
			break;
		case 5:
			!newCS.isCreated ? throw "There's no added CS's\n" : EditCS(CSToEdit);
			break;
		case 6:
			std::cout << "Enter path to location for save: \n";
			std::cin >> filepath;
			Save(filepath, pipeToEdit, CSToEdit);
			break;
		case 0:
			break;
		default: std::cout << "There's no such an option!\n";
		}
	}
}