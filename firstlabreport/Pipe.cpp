#include "Pipe.h"
#include "utilities.h"
#include <iostream>
#include <fstream>

int Pipe::maxID = 0;

Pipe::Pipe() : pipeName(""), length(0.0f), diameter(0), isRepairing(false), pipeID(maxID) {}

std::string Pipe::GetName() const {
	return pipeName;
}

int Pipe::GetID() const {
	return pipeID;
}

bool Pipe::GetRepairingStatus() const {
	return isRepairing;
}

void Pipe::EditPipe() {
	isRepairing = !isRepairing;
}

std::istream& operator>>(std::istream& in, Pipe& pipe) {

	pipe.pipeID = ++pipe.maxID;
	std::cout << "Enter pipe name: \n";
	std::getline(in >> std::ws, pipe.pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	pipe.length = GetCorrectInput(5.0, 12.0);
	std::cout << "Enter pipe diameter: (in millimetres): \n";
	pipe.diameter = GetCorrectInput(500, 1420);
	std::cout << "Enter pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	pipe.isRepairing = GetCorrectInput(0, 1);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Pipe& pipe) {
	out << "----- Pipe specs -----\n";
	out << "ID: " << pipe.pipeID << '\n' << "Name: " << pipe.pipeName << '\n' <<
		"Length: " << pipe.length << " m." << '\n' <<
		"Diameter: " << pipe.diameter << " mm." << '\n';
	out << "Repairing status: " << (pipe.isRepairing ? "not in repairing\n\n" : "in repairing\n\n");
	return out;
}

std::ofstream& operator<< (std::ofstream& fout, const Pipe& pipe) {
	fout << pipe.pipeID << '\n' << pipe.pipeName << '\n' << pipe.length << '\n' <<
		pipe.diameter << '\n' << pipe.isRepairing << '\n';
	return fout;
}

std::ifstream& operator>> (std::ifstream& fin, Pipe& pipe) {
	fin >> pipe.pipeID;
	std::getline(fin >> std::ws, pipe.pipeName) >> pipe.length >> 
		pipe.diameter >> pipe.isRepairing;
	pipe.maxID = pipe.pipeID;
	return fin;
}

