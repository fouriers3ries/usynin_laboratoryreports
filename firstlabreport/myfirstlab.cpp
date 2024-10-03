#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Pipe {
	std::string pipeName = "";
	float length = 0.0f;
	float diameter = 0.0f;
	bool isRepairing = false;
};

struct CS {
	std::string CSName = "";
	int workshopsQuantity = 0;
	int busyWorkshopsQuantity = 0;
	float effectiveness = 0.0f;
};

bool isUserChoiceCorrect(int choice) {
	return (choice == 0 || choice >= 1 && choice <= 9) ? true : false;
}

void ShowMenu() {
	std::cout << "First laboratory work report by Usynin Daniil (AS-23-04)\n\n" <<
		"Choose an option to do:\n" << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Edit pipe\n" << "5. Edit CS\n"
		<< "6. Save\n" << "7. Load\n" << "0. Exit\n\n";
};

// overloading output operator for pipe specs printing
std::ostream& operator<< (std::ostream& out, const Pipe& pipe) {
	out << "----- Pipe specs -----\n";
	out << "Name: " << pipe.pipeName << '\n' <<
		"Length: " << pipe.length << " m." << '\n' <<
		"Diameter: " << pipe.diameter << " mm." << '\n';
	std::string status = pipe.isRepairing ? "not in repairing\n\n" : "in repairing\n\n";
	out << "Repairing status: " << status;
	return out;
};

// overloading output operator for CS specs printing
std::ostream& operator<< (std::ostream& out, const CS& CS) {
	out << "----- CS specs -----\n";
	out << "Name: " << CS.CSName << '\n' <<
		"Workshops quantity: " << CS.workshopsQuantity << '\n' <<
		"Busy workshops quantity: " << CS.busyWorkshopsQuantity << '\n' <<
		"Effectiveness: " << CS.effectiveness << '\n';
	return out;
};

// function for printing errors according to its ID - errorcode
void ShowError(std::istream& in, int errorcode) {
	switch (errorcode) {
	case 0:
		std::cout << "Choose an option from the list!\n";
		break;
	case 1:
		std::cout << "Error! Enter a positive integer!\n";
		break;
	case 2:
		std::cout << "Error! Enter a positive number!\n";
		break;
	case 3:
		std::cout << "Error! Choose 1 or 0!\n";
		break;
	case 4:
		std::cout << "Error! Enter a number between 0.00 and 1.00\n";
		break;
	case 5:
		std::cout << "Error! Busy workshop quantity shouldn't be greater than total workshops quantity!\n";
		break;
	}
	in.clear();
	in.ignore(32767, '\n');
}

std::istream& operator>> (std::istream& in, Pipe& pipe) {
	std::cout << "Enter pipe name: \n";
	std::getline(in >> std::ws, pipe.pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	in >> pipe.length;
	while (in.fail() || pipe.length <= 0) {
		ShowError(in, 1);
		in >> pipe.length;
	}
	std::cout << "Enter pipe diameter (in millimetres): \n";
	in >> pipe.diameter;
	while (in.fail() || pipe.diameter <= 0) {
		ShowError(in, 2);
		in >> pipe.diameter;
	}
	std::cout << "Enter pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	int status;
	in >> status;
	while (in.fail() || (!(status == 1) && !(status == 0))) {
		ShowError(in, 3);
		in >> status;
	}
	pipe.isRepairing = status;

	return in;
};

std::istream& operator>> (std::istream& in, CS& CS) {
	std::cout << "Enter CS name: \n";
	std::getline(in >> std::ws, CS.CSName);
	std::cout << "Enter workshops quantity: \n";
	in >> CS.workshopsQuantity;
	while (in.fail() || CS.workshopsQuantity <= 0) {
		ShowError(in, 1);
		in >> CS.workshopsQuantity;
	}
	std::cout << "Enter busy workshops quantity: \n";
	in >> CS.busyWorkshopsQuantity;
	while (in.fail() || CS.busyWorkshopsQuantity <= 0 || CS.busyWorkshopsQuantity > CS.workshopsQuantity) {
		if (CS.busyWorkshopsQuantity > CS.workshopsQuantity) {
			ShowError(in, 5);
			in >> CS.busyWorkshopsQuantity;
		}
		else {
			ShowError(in, 2);
			in >> CS.busyWorkshopsQuantity;
		}
	}
	std::cout << "Enter workshops effectiveness (from 0.00 to 1.00): \n";
	in >> CS.effectiveness;
	while (in.fail() || CS.effectiveness <= 0 || CS.effectiveness > 1) {
		ShowError(in, 4);
		in >> CS.effectiveness;
	}
	return in;
};

void EditPipe(Pipe& pipeToEdit) {
	std::cout << "Enter new pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	int status;
	std::cin >> status;
	while (std::cin.fail() || (!(status == 1) && !(status == 0))) {
		ShowError(std::cin, 3);
		std::cin >> status;
	}
	pipeToEdit.isRepairing = status;
}

void EditCS(CS& CSToEdit) {
	std::cout << "Enter new busy workshops quantity: \n";
	std::cin >> CSToEdit.busyWorkshopsQuantity;
	while (std::cin.fail() || CSToEdit.busyWorkshopsQuantity <= 0 || CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) {
		if (CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) {
			ShowError(std::cin, 5);
			std::cin >> CSToEdit.busyWorkshopsQuantity;
		}
		else {
			ShowError(std::cin, 2);
			std::cin >> CSToEdit.busyWorkshopsQuantity;
		}
	}
}

void SavePipe(std::ofstream& file, const Pipe& pipeToSave) {
	file.open("data.txt");
	if (file.is_open()) {
		file << pipeToSave.pipeName << '\n' << pipeToSave.length << '\n' <<
			pipeToSave.diameter << '\n';
		file << (pipeToSave.isRepairing ? "not in repairing\n" : "in repairing\n");
		file.close();
	}
	else std::cout << "Error! File cannot be open!\n";
}

void SaveCS(std::ofstream& file, const CS& CSToSave) {
	file.open("data.txt", std::ios::app);
	if (file.is_open()) {
		file << "Information about CS" << '\n';
		file << CSToSave.CSName << '\n' << CSToSave.workshopsQuantity << '\n' <<
			CSToSave.busyWorkshopsQuantity << '\n' << CSToSave.effectiveness;
		file.close();
	}
	else std::cout << "Error! File cannot be open!\n";
}

void Save(std::ofstream& file, const Pipe& pipeToSave, const CS& CSToSave) {
	if (pipeToSave.length == 0) std::cout << "Pipe data was not written (there's no added pipe)\n";
	if (CSToSave.workshopsQuantity == 0) std::cout << "CS data was not written(there's no added CS)\n";
	SavePipe(file, pipeToSave);
	SaveCS(file, CSToSave);
}

void LoadPipe(std::ifstream& file, Pipe& pipeToEdit) {
	std::string temp = "";
	file.open("data.txt");
	if (file.is_open()) {
		std::getline(file, pipeToEdit.pipeName);
		std::getline(file, temp);
		pipeToEdit.length = std::stof(temp.c_str());
		std::getline(file, temp);
		pipeToEdit.diameter = std::stof(temp.c_str());
		std::getline(file, temp);
		pipeToEdit.isRepairing = temp == "not in repairing" ? true : false;
	}
	else {
		std::cout << "Error! File cannot be open!\n";
	}
	file.close();

}

void LoadCS(std::ifstream& file, CS& CSToEdit) {
	file.open("data.txt");
	std::string temp;
	if (file.is_open()) {
		while (temp != "Information about CS") {
			std::getline(file, temp);
		}
		std::getline(file, CSToEdit.CSName);
		std::getline(file, temp);
		CSToEdit.workshopsQuantity = std::stoi(temp.c_str());
		std::getline(file, temp);
		CSToEdit.busyWorkshopsQuantity = std::stoi(temp.c_str());
		std::getline(file, temp);
		CSToEdit.effectiveness = std::stof(temp.c_str());
	}
	file.close();
}

void Load(std::ifstream& file, Pipe& pipeToEdit, CS& CSToEdit) {
	LoadPipe(file, pipeToEdit);
	LoadCS(file, CSToEdit);
}

int main() {
	int userChoice;
	Pipe pipe = {};
	CS CS = {};
	std::ofstream file;
	std::ifstream file_1;
	for (;;) {
		ShowMenu();
		std::cin >> userChoice;
		while (!isUserChoiceCorrect(userChoice) || std::cin.fail()) {
			ShowError(std::cin, 0);
			std::cin >> userChoice;
		}
		switch (userChoice) {
		case 1:
			std::cin >> pipe;
			continue;
		case 2:
			std::cin >> CS;
			continue;
		case 3:
			if (pipe.length == 0) std::cout << "There's no added pipe\n";
			else std::cout << pipe;
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else std::cout << CS;
			continue;
		case 4:
			if (pipe.length == 0) std::cout << "There's no added pipe\n";
			else EditPipe(pipe);
			continue;
		case 5:
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else EditCS(CS);
			continue;
		case 6:
			Save(file, pipe, CS);
			continue;
		case 7:
			Load(file_1, pipe, CS);
			continue;
		case 0:
			break;

		}
		break;
	}
}