#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Pipe {
	std::string pipeName = "";
	float length = 0.0f;
	int diameter = 0;
	bool isRepairing = false;
};

struct CS {
	std::string CSName = "";
	int workshopsQuantity = 0;
	int busyWorkshopsQuantity = 0;
	float effectiveness = 0.0f;
};

bool isUserChoiceCorrect(int choice) {
	return (choice == 0 || choice >= 1 && choice <= 9);
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
	out << "Repairing status: " << (pipe.isRepairing ? "not in repairing\n\n" : "in repairing\n\n");
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

// function for printing errors according to its errorcode
void ProcessError(int errorcode) {
	std::vector<std::string> errors{ "Choose an option from the list!\n", "Error! Enter a positive integer!\n" ,
	"Error! Enter a positive number!\n", "Error! Choose 1 or 0!\n", "Error! Enter a number between 0.00 and 1.00\n",
	"Error! Busy workshop quantity shouldn't be greater than total workshops quantity!\n" };
	std::cout << errors[errorcode];
	std::cin.clear();
	std::cin.ignore(32767, '\n');
}

void EditPipe(Pipe& pipeToEdit) {
	std::cout << "Enter pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	std::cin >> pipeToEdit.isRepairing;
	while (std::cin.fail()) {
		ProcessError(3);
		std::cin >> pipeToEdit.isRepairing;
	}
}

void EditCS(CS& CSToEdit) {
	std::cout << "Enter busy workshops quantity: \n";
	std::cin >> CSToEdit.busyWorkshopsQuantity;
	while (std::cin.fail() || CSToEdit.busyWorkshopsQuantity <= 0 || CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) {
		(CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) ? ProcessError(5) : ProcessError(2);
		std::cin >> CSToEdit.busyWorkshopsQuantity;
	}
}

std::istream& operator>> (std::istream& in, Pipe& pipe) {
	std::cout << "Enter pipe name: \n";
	std::getline(in >> std::ws, pipe.pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	in >> pipe.length;
	while (in.fail() || pipe.length <= 0) {
		ProcessError(1);
		in >> pipe.length;
	}
	std::cout << "Enter pipe diameter (in millimetres): \n";
	in >> pipe.diameter;
	while (in.fail() || pipe.diameter <= 0) {
		ProcessError(2);
		in >> pipe.diameter;
	}
	EditPipe(pipe);

	return in;
};

std::istream& operator>> (std::istream& in, CS& CS) {
	std::cout << "Enter CS name: \n";
	std::getline(in >> std::ws, CS.CSName);
	std::cout << "Enter workshops quantity: \n";
	in >> CS.workshopsQuantity;
	while (in.fail() || CS.workshopsQuantity <= 0) {
		ProcessError(1);
		in >> CS.workshopsQuantity;
	}
	EditCS(CS);
	std::cout << "Enter workshops effectiveness (from 0.00 to 1.00): \n";
	in >> CS.effectiveness;
	while (in.fail() || CS.effectiveness <= 0 || CS.effectiveness > 1) {
		ProcessError(4);
		in >> CS.effectiveness;
	}
	return in;
};

void SavePipe(const Pipe& pipeToSave, std::ofstream& file) {
	file << pipeToSave.pipeName << '\n' << pipeToSave.length << '\n' <<
		pipeToSave.diameter << '\n' << pipeToSave.isRepairing << '\n';
}

void SaveCS(const CS& CSToSave, std::ofstream& file) {
	file << CSToSave.CSName << '\n' << CSToSave.workshopsQuantity << '\n' <<
		CSToSave.busyWorkshopsQuantity << '\n' << CSToSave.effectiveness;
}

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

void LoadPipe(Pipe& pipeToEdit, std::ifstream& file) {
	std::getline(file >> std::ws, pipeToEdit.pipeName);
	file >> pipeToEdit.length >> pipeToEdit.diameter >> pipeToEdit.isRepairing;
}

void LoadCS(CS& CSToEdit, std::ifstream& file) {
	std::getline(file >> std::ws, CSToEdit.CSName);
	file >> CSToEdit.workshopsQuantity >> CSToEdit.busyWorkshopsQuantity >>
		CSToEdit.effectiveness;
}

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

int main() {
	int userChoice;
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
			if (pipe.length == 0) std::cout << "There's no added pipe\n";
			else std::cout << pipe;
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else std::cout << CS;
			break;
		case 4:
			if (pipe.length == 0) std::cout << "There's no added pipe\n";
			else EditPipe(pipe);
			break;
		case 5:
			if (CS.workshopsQuantity == 0) std::cout << "There's no added CS\n";
			else EditCS(CS);
			break;
		case 6:
			Save(pipe, CS);
			break;
		case 7:
			Load(pipe, CS);
			break;
		case 0:
			return 0;
		}

	}
}