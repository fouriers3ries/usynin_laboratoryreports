#pragma once
#include <string>
#include <iostream>
#include <fstream>

class CS {

private:
	std::string CSName;
	int workshopsQuantity;
	int busyWorkshopsQuantity;
	float effectiveness;
	int csID;
	static int maxID;

public:
	CS();

	// get things
	std::string GetName() const;
	float GetAFK() const; // get percentage of unused workshops 
	int GetID() const;

	// change busy workshops quantity
	void EditCS();

	friend std::ifstream& operator>> (std::ifstream& fin, CS& CS);
	friend std::ofstream& operator<< (std::ofstream& fout, const CS& CS);

	friend std::istream& operator>>(std::istream& in, CS& CS);
	friend std::ostream& operator<<(std::ostream& out, const CS& CS);


};