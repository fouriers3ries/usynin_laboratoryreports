#include "CS.h"
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <string>

int CS::maxID = 0;

CS::CS() : CSName(""), workshopsQuantity(0), busyWorkshopsQuantity(0), effectiveness(0.0f), csID(maxID) {}

std::string CS::GetName() const {
	return CSName;
}

float CS::GetAFK() const {
	return 100 * (1 - busyWorkshopsQuantity / workshopsQuantity);
}

int CS::GetID() const {
	return csID;
}

void CS::EditCS() {
	std::cout << "Enter busy workshops quantity: \n";
	workshopsQuantity = GetCorrectInput(0, workshopsQuantity);
}

std::istream& operator>> (std::istream& in, CS& CS) {
	CS.csID = ++CS.maxID;
	std::cout << "Enter CS name: \n";
	std::getline(in >> std::ws, CS.CSName);
	std::cout << "Enter workshops quantity: \n";
	CS.workshopsQuantity = GetCorrectInput(1, 1000);
	std::cout << "Enter busy workshops quantity: \n";
	CS.busyWorkshopsQuantity = GetCorrectInput(0, CS.workshopsQuantity);
	std::cout << "Enter workshops effectiveness (from 0.00 to 1.00): \n";
	CS.effectiveness = GetCorrectInput(0.00, 1.00);
	return in;
};

std::ostream& operator<< (std::ostream& out, const CS& CS) {
	out << "----- CS specs -----\n";
	out << "ID: " << CS.csID << '\n' << "Name: " << CS.CSName << '\n' <<
		"Workshops quantity: " << CS.workshopsQuantity << '\n' <<
		"Busy workshops quantity: " << CS.busyWorkshopsQuantity << '\n' <<
		"Effectiveness: " << CS.effectiveness << '\n';
	return out;
};

std::ofstream& operator<< (std::ofstream& fout, const CS& CS) {
	fout << CS.CSName << '\n' << CS.workshopsQuantity << '\n' <<
		CS.busyWorkshopsQuantity << '\n' << CS.effectiveness;
	return fout;
}

std::ifstream& operator>> (std::ifstream& fin, CS& CS) {
	fin >> CS.csID;
	std::getline(fin >> std::ws, CS.CSName);
	fin >> CS.workshopsQuantity >> CS.busyWorkshopsQuantity >> CS.effectiveness;
	CS.maxID = CS.csID;
	return fin;
}



