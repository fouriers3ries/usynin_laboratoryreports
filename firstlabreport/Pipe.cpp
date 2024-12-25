#include "Pipe.h"
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <string>

int Pipe::maxID = 0;

Pipe::Pipe() : pipeName(""), length(0.0f), diameter(0), isRepairing(false), isFree(false), pipeID(maxID) {}

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

int Pipe::GetDiameter() const {
	return diameter;
}

void Pipe::setFree() {
	isFree = !isFree;
}

bool Pipe::isConnected() const {
	return isFree;
}

float Pipe::GetLength() const {
	return length;
}

float calculatePipePerformance(float length, int diameter, int coef = 1000) {
	return coef * sqrt(pow(diameter * 0.001, 5) / length);
}

float Pipe::GetPerformance() const {
	return performance;
}

void Pipe::SetPerformance(float performance) {
	this->performance = performance;
}

void Pipe::CreateSuitablePipe(std::unordered_map<int, Pipe>& destination, int diameter) {
	pipeID = ++maxID;
	this->diameter = diameter;
	this->isRepairing = true;

	std::cout << "Enter pipe name: \n";
	std::getline(std::cin >> std::ws, pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	length = GetCorrectInput(0, INT_MAX);

	this->performance = calculatePipePerformance(length, diameter);
	destination.emplace(GetID(), *this);
}

void Pipe::CreateReversePipe(std::unordered_map<int, Pipe>& destination, int forwardPipeID, const Pipe& forwardPipe) {
	pipeID = ++maxID; // Уникальный идентификатор для обратной трубы
	pipeName = "Reverse of " + forwardPipe.GetName(); // Название обратной трубы
	length = forwardPipe.GetLength(); // Длина обратной трубы такая же, как у исходной
	diameter = forwardPipe.GetDiameter(); // Диаметр остается прежним
	isRepairing = false; // Обратное ребро по умолчанию не в ремонте
	performance = 0; // Изначально пропускная способность обратного ребра равна 0

	// Добавляем обратную трубу в указанный контейнер
	destination.emplace(pipeID, *this);
}

std::istream& operator>>(std::istream& in, Pipe& pipe) {

	pipe.pipeID = ++pipe.maxID;
	std::cout << "Enter pipe name: \n";
	std::getline(in >> std::ws, pipe.pipeName);
	std::cout << "Enter pipe length (in metres): \n";
	pipe.length = GetCorrectInput(0, INT_MAX);
	std::cout << "Enter pipe diameter: (in millimetres): \n";
	pipe.diameter = GetCorrectInput(100, 2000);
	std::cout << "Enter pipe repairing status (1 - not in repairing, 0 - in repairing): \n";
	pipe.isRepairing = GetCorrectInput(0, 1);
	pipe.performance = calculatePipePerformance(pipe.length, pipe.diameter);
	return in;
}

std::ostream& operator<<(std::ostream& out, const Pipe& pipe) {
	out << "----- Pipe specs -----\n";
	out << "ID: " << pipe.pipeID << '\n' << "Name: " << pipe.pipeName << '\n' <<
		"Length: " << pipe.length << " m." << '\n' <<
		"Diameter: " << pipe.diameter << " mm." << '\n';
	out << "Repairing status: " << (pipe.isRepairing ? "not in repairing\n" : "in repairing\n");
	out << "Connection status: " << (pipe.isFree ? "connected\n\n" : "not connected\n\n");
	return out;
}

std::ofstream& operator<< (std::ofstream& fout, const Pipe& pipe) {
	fout << pipe.pipeID << '\n' << pipe.pipeName << '\n' << pipe.length << '\n' <<
		pipe.diameter << '\n' << pipe.isRepairing << '\n' << pipe.isFree << '\n' << pipe.performance << '\n';
	return fout;
}

std::ifstream& operator>> (std::ifstream& fin, Pipe& pipe) {
	fin >> pipe.pipeID;
	std::getline(fin >> std::ws, pipe.pipeName) >> pipe.length >>
		pipe.diameter >> pipe.isRepairing >> pipe.isFree >> pipe.performance;
	pipe.maxID = pipe.pipeID;
	return fin;
}