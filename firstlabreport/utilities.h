#pragma once
#include <iostream>
#include <unordered_map>

// template for getting correct input from the user
template <typename T>
T GetCorrectInput(T min, T max) {
	T value;
	while ((std::cin >> value).fail() || value < min || value > max) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "\nfuck you. input must be a value between " << min << " and " << max << '\n';
		std::cout << "enter your fucking value: ";
	}
	return value;
};

// function for printing errors according to its errorkey & do some cin thread cleaning stuff
void ProcessError(std::string errorkey) {
	std::unordered_map<std::string, std::string> errors = {
		{"M0", "Choose an option from the list!\n"},
		{"CS0", "Error! Enter a number between 0.00 and 1.00\n"},
		{"CS1", "Error! Busy workshop quantity shouldn't be greater than total workshops quantity!\n"},
		{"P0", "Error! Choose 1 or 0!\n"},
		{"G0", "Error! Enter a positive mumber!\n"},
	};

	std::cout << errors[errorkey];
	std::cin.clear();
	std::cin.ignore(32767, '\n');
}

bool isUserChoiceCorrect(int choice) {
	return (choice == 0 || choice >= 1 && choice <= 9);
}