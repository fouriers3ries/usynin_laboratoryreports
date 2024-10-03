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

// function for printing errors according to its errorcode
void ProcessError(std::istream& in, int errorcode) {//!!!!!!!!!!!!!!!
	std::vector<std::string> errors{ "Choose an option from the list!\n", "Error! Enter a positive integer!\n" ,
	"Error! Enter a positive number!\n", "Error! Choose 1 or 0!\n", "Error! Enter a number between 0.00 and 1.00\n",
	"Error! Busy workshop quantity shouldn't be greater than total workshops quantity!\n" };
	std::cout << errors[errorcode];
	in.clear();
	in.ignore(32767, '\n');
}

void EditPipe(Pipe& pipeToEdit) {
	std::cout << "Enter pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	std::cin >> pipeToEdit.isRepairing;
	while (std::cin.fail()) {
		ProcessError(std::cin, 3);
		std::cin >> pipeToEdit.isRepairing;
	}
}

void EditCS(CS& CSToEdit) {
	std::cout << "Enter busy workshops quantity: \n";
	std::cin >> CSToEdit.busyWorkshopsQuantity;
	while (std::cin.fail() || CSToEdit.busyWorkshopsQuantity <= 0 || CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) {
		(CSToEdit.busyWorkshopsQuantity > CSToEdit.workshopsQuantity) ? ProcessError(std::cin, 5) : ProcessError(std::cin, 2);
		std::cin >> CSToEdit.busyWorkshopsQuantity;
	}
}

std::istream& operator>> (std::istream& in, Pipe& pipe) {
	std::cout << "Enter pipe name: \n";
	std::getline(in >> std::ws, pipe.pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	in >> pipe.length;
	while (in.fail() || pipe.length <= 0) {
		ProcessError(in, 1);
		in >> pipe.length;
	}
	std::cout << "Enter pipe diameter (in millimetres): \n";
	in >> pipe.diameter;
	while (in.fail() || pipe.diameter <= 0) {
		ProcessError(in, 2);
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
		ProcessError(in, 1);
		in >> CS.workshopsQuantity;
	}
	EditCS(CS);
	std::cout << "Enter workshops effectiveness (from 0.00 to 1.00): \n";
	in >> CS.effectiveness;
	while (in.fail() || CS.effectiveness <= 0 || CS.effectiveness > 1) {
		ProcessError(in, 4);
		in >> CS.effectiveness;
	}
	return in;
};

void SavePipe(const Pipe& pipeToSave) {
	std::ofstream file("data.txt");
	if (file.is_open()) {
		file << pipeToSave.pipeName << '\n' << pipeToSave.length << '\n' <<
			pipeToSave.diameter << '\n';
		file << (pipeToSave.isRepairing ? "not in repairing\n" : "in repairing\n");
		file.close();
	}
	else std::cout << "Error! File cannot be open!\n";
}

void SaveCS(const CS& CSToSave) {
	std::ofstream file;
	file.open("data.txt", std::ios::app);
	if (file.is_open()) {
		file << "Information about CS" << '\n';
		if (CSToSave.CSName != "") {
			file << CSToSave.CSName << '\n' << CSToSave.workshopsQuantity << '\n' <<
				CSToSave.busyWorkshopsQuantity << '\n' << CSToSave.effectiveness;
			file.close();
		}
	}
	else std::cout << "Error! File cannot be open!\n";
}

void Save(const Pipe& pipeToSave, const CS& CSToSave) {//!!!!!!!!!!!!!!
	std::ofstream file("data.txt");
	if (pipeToSave.length == 0) std::cout << "Pipe data was not written (there's no added pipe)\n";
	if (CSToSave.workshopsQuantity == 0) std::cout << "CS data was not written(there's no added CS)\n";
	SavePipe(pipeToSave);
	SaveCS(CSToSave);
}

void LoadPipe(Pipe& pipeToEdit) {
	std::ifstream file("data.txt");
	std::string temp;
	file >> temp;
	if (file.is_open()) {
		if (temp != "") {
			file >> pipeToEdit.pipeName;
			file >> pipeToEdit.length;
			file >> pipeToEdit.diameter;
			file >> pipeToEdit.isRepairing;
		}
	}
	else {
		std::cout << "Error! File cannot be open!\n";
	}
	file.close();

}

void LoadCS(CS& CSToEdit) {
	std::ifstream file("data.txt");
	std::string temp = "";
	if (file.is_open()) {
		while (temp != "Information about CS") {//!!!!!!!!
			file >> temp;
		}
		if (temp != "") {
			file >> CSToEdit.CSName;
			file >> CSToEdit.workshopsQuantity;
			file >> CSToEdit.busyWorkshopsQuantity;
			file >> CSToEdit.effectiveness;
		}
	}
	file.close();
}

void Load(Pipe& pipeToEdit, CS& CSToEdit) {
	std::ofstream file("data.txt");
	LoadPipe(pipeToEdit);
	LoadCS(CSToEdit);
}

int main() {
	int userChoice;
	Pipe pipe = {};
	CS CS = {};
	for (;;) {
		ShowMenu();
		std::cin >> userChoice;
		while (!isUserChoiceCorrect(userChoice) || std::cin.fail()) {
			ProcessError(std::cin, 0);
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