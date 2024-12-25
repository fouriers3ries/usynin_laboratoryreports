#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>


class Pipe {

private:
	std::string pipeName;
	float length;
	int diameter;
	bool isRepairing;
	int pipeID;
	bool isFree;
	static int maxID;

public:
	Pipe();

	// get things
	std::string GetName() const;
	bool GetRepairingStatus() const;
	int GetID() const;
	int GetDiameter() const;
	bool isConnected() const;

	void EditPipe();
	void setFree();
	void SetDiameter();
	void CreateSuitablePipe(std::unordered_map<int, Pipe>& destination, int diameter);

	friend std::ifstream& operator>> (std::ifstream& fin, Pipe& pipe);
	friend std::ofstream& operator<< (std::ofstream& fout, const Pipe& pipe);

	friend std::istream& operator>>(std::istream& in, Pipe& pipe);
	friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);

};