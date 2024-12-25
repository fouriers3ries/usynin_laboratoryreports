#include <iostream>
#include <fstream>
#include <string>
#include "Pipe.h"
#include "CS.h"
#include "utilities.h"
#include <unordered_map>
#include <chrono>
#include <format>
#include "GTN.h"

void ShowMenu() {
	std::cout << "Third laboratory work report by Usynin Daniil (AS-23-04)\n\n" <<
		"Choose an option to do:\n" << "1. Add pipe\n" << "2. Add CS\n" <<
		"3. Show all objects\n" << "4. Search for object\n" << "5. Connect pipe and CS in GTN\n" <<
		"6. Show GTN\n" << "7. Delete GTN\n" << "8. Topological sorting\n" << "9. Save\n" << "10. Load\n" << 
		"11. Highest flow in network\n" << "12. Shortest path\n" << "0. Exit\n\n";
};

void ShowSearchMenu() {
	std::cout << "1. Search for pipe by name\n" << "2. Search for pipe by repairing status\n" <<
		"3. Search for CS by name\n" << "4. Search for CS by workshops\n" << "0. Exit to main menu\n";
}

void ShowQuantityMenu() {
	std::cout << "1. Perform action by ID\n" << "2. Perform action with all objects\n" << "0. Back\n";
}

void ShowPurposeMenu() {
	std::cout << "1. Edit\n" << "2. Delete\n" << "0. Back\n";
}

template<typename T>
void SaveObject(std::ofstream& path, std::unordered_map<int, T>& objectPackage) {
	for (auto& object : objectPackage) {
		path << object.second;
	}
}

void SaveGraph(std::ofstream& path, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	for (const auto& [startCSID, edges] : graph) {
		for (const auto& [endCSID, pipeID] : edges) {
			path << startCSID << " " << endCSID << " " << pipeID << "\n";
		}
	}
}

void LoadGraph(std::ifstream& path, std::unordered_map<int, std::unordered_map<int, int>>& graph, std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage) {
	graph.clear();
	int startCSID, endCSID, pipeID;
	while (path >> startCSID >> endCSID >> pipeID) {
		if (CSPackage.find(startCSID) != CSPackage.end() &&
			CSPackage.find(endCSID) != CSPackage.end() &&
			pipePackage.find(pipeID) != pipePackage.end()) {
			graph[startCSID][endCSID] = pipeID;
		}
		else {
			std::cerr << "Warning: Invalid data found in file (CS or Pipe not found). Skipping: "
				<< startCSID << " " << endCSID << " " << pipeID << "\n";
		}
	}
}

template<typename T>
std::unordered_map<int, T> LoadObject(std::ifstream& inputFile, std::unordered_map<int, T>& objectPackage, int quantity) {
	for (size_t i = 0; i < quantity; i++) {
		T object;
		inputFile >> object;
		objectPackage.emplace(object.GetID(), object);
	}
	return objectPackage;
}

void Load(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	std::ifstream inputFile;
	std::string path;

	std::cout << "Enter file name: \n";
	std::getline(std::cin >> std::ws, path);
	inputFile.open(path + ".txt.");

	if (inputFile.is_open()) {
		int pipesQuantity, CSQuantity;
		inputFile >> pipesQuantity;
		LoadObject(inputFile, pipePackage, pipesQuantity);
		inputFile >> CSQuantity;
		LoadObject(inputFile, CSPackage, CSQuantity);
		LoadGraph(inputFile, graph, pipePackage, CSPackage);
		std::cout << "Data loaded successfully\n";
	}
	else std::cout << "There're some problems during reading from file. Try again.\n";
	inputFile.close();
}

void Save(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	std::string path{};
	std::ofstream outputFile{};
	std::cout << "Enter file name: \n";
	INPUT_LINE(std::cin, path);
	outputFile.open(path + ".txt", std::ios::app);
	if (outputFile.is_open()) {
		outputFile << pipePackage.size() << '\n';
		SaveObject(outputFile, pipePackage);
		outputFile << CSPackage.size() << '\n';
		SaveObject(outputFile, CSPackage);
		SaveGraph(outputFile, graph);
		std::cout << "Data writed to file successfully\n";
	}
	else std::cout << "Failed! Data cannot be written.";
	outputFile.close();
}

void EditPipesByChoice(std::unordered_map<int, Pipe>& pipePackage, std::unordered_set<int>& searchResult, int userChoice) {
	if (userChoice == 1) PipePackageEdit(pipePackage, searchResult);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			pipePackage.at(ID).EditPipe();
		}
		std::cout << "All suitable pipes was successfully edited\n";
	}
}

void EditCSByChoice(std::unordered_map<int, CS>& CSPackage, std::unordered_set<int>& searchResult, int userChoice) {
	if (userChoice == 1) CSPackageEdit(CSPackage, searchResult);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			CSPackage.at(ID).EditCS();
		}
		std::cout << "All suitable CS's was successfully edited\n";
	}
}

template<typename T>
void DeleteObjectsByChoice(std::unordered_map<int, T>& objectPackage, std::unordered_set<int>& searchResult, int userChoice, std::unordered_map<int, std::unordered_map<int, int>>& graph, bool isPipe = false) {
	if (userChoice == 1) ObjectPackageDelete(objectPackage, searchResult, graph, isPipe);
	if (userChoice == 2) {
		for (int ID : searchResult) {
			auto obj = objectPackage.find(ID);
			objectPackage.erase(obj);
		}
		std::cout << "All suitable objects was successfully deleted\n";
	}
}

void ShowObjects(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, CS>& CSPackage) {
	if (pipePackage.empty())
		std::cout << "There's no added pipes!\n";
	else
		for (const auto& pipe : pipePackage)
			std::cout << pipe.second << '\n';
	if (CSPackage.empty())
		std::cout << "There's no added CS's!\n";
	else
		for (const auto& CS : CSPackage)
			std::cout << CS.second << '\n';
}

template<typename T>
void AddObject(std::unordered_map<int, T>& objectPackage) {
	T objToAdd;
	std::cin >> objToAdd;
	objectPackage.emplace(objToAdd.GetID(), objToAdd);
}

void SearchPipesByName(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	std::string searchingName{};
	std::cout << "Enter name for searching: \n";
	INPUT_LINE(std::cin, searchingName);
	auto searchResult = FindPipeByFilter(pipePackage, CheckPipeByName, searchingName);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditPipesByChoice(pipePackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 0) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(pipePackage, searchResult, userChoiceForQuantity, graph, true);
		}
	}
	else std::cout << "There's no pipes with that name!" << '\n';
}

void SearchPipesByStatus(std::unordered_map<int, Pipe>& pipePackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	bool searchingStatus{};
	std::cout << "Enter pipe repairing status for searching (1 - not in repairing, 0 - in repairing): \n";
	searchingStatus = GetCorrectInput(0, 1);
	auto searchResult = FindPipeByFilter(pipePackage, CheckPipeByStatus, searchingStatus);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditPipesByChoice(pipePackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 0) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(pipePackage, searchResult, userChoiceForQuantity, graph, true);
		}
	}
	else std::cout << "There's no pipes with that status!\n";
}

void SearchCSByName(std::unordered_map<int, CS>& CSPackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	std::string searchingName{};
	std::cout << "Enter name for searching: \n";
	INPUT_LINE(std::cin, searchingName);
	auto searchResult = FindCSByFilter(CSPackage, CheckCSByName, searchingName);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditCSByChoice(CSPackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 3) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(CSPackage, searchResult, userChoiceForQuantity, graph);
		}
	}
	else std::cout << "There's no CS's with that name!\n";
}

void SearchCSByStatus(std::unordered_map<int, CS>& CSPackage, std::unordered_map<int, std::unordered_map<int, int>>& graph) {
	int searchingPercentage{};
	std::cout << "Enter disability percentage for searching: \n";
	searchingPercentage = GetCorrectInput(0, 100);
	auto searchResult = FindCSByFilter(CSPackage, CheckCSByAFK, searchingPercentage);
	if (!searchResult.empty()) {
		ShowPurposeMenu();
		int userChoice = GetCorrectInput(0, 2);
		if (userChoice == 1) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			EditCSByChoice(CSPackage, searchResult, userChoiceForQuantity);
			if (userChoiceForQuantity == 3) return;
		};
		if (userChoice == 2) {
			ShowQuantityMenu();
			int userChoiceForQuantity = GetCorrectInput(0, 2);
			DeleteObjectsByChoice(CSPackage, searchResult, userChoiceForQuantity, graph);
		}
	}
	else std::cout << "There's no CS's with that disability percentage!\n";
}

void MainMenu() {
	GTN network;
	std::unordered_set<int> output;

	while (true) {
		ShowMenu();
		switch (GetCorrectInput(0, 12)) {
		case 1: {
			AddObject(network.GetPipePackage());
			break;
		}
		case 2: {
			AddObject(network.GetCSPackage());
			break;
		}
		case 3: {
			ShowObjects(network.GetPipePackage(), network.GetCSPackage());
			break;
		}
		case 4: {
			bool exitSearchMenu = false;
			while (!exitSearchMenu) {
				ShowSearchMenu();
				switch (GetCorrectInput(0, 4)) {
				case 1:
					if (network.GetPipePackage().empty())
						std::cout << "There's no added pipes\n";
					else
						SearchPipesByName(network.GetPipePackage(), network.getGraphMat());
					break;
				case 2:
					if (network.GetPipePackage().empty())
						std::cout << "There's no added pipes\n";
					else
						SearchPipesByStatus(network.GetPipePackage(), network.getGraphMat());
					break;
				case 3:
					if (network.GetCSPackage().empty())
						std::cout << "There's no added CS's\n";
					else
						SearchCSByName(network.GetCSPackage(), network.getGraphMat());
					break;
				case 4:
					if (network.GetCSPackage().empty())
						std::cout << "There's no added CS's\n";
					else
						SearchCSByStatus(network.GetCSPackage(), network.getGraphMat());
					break;
				case 0:
					exitSearchMenu = true;
					break;
				}
			}
			break;
		}
		case 5: {
			int startCSID, endCSID, diameter;
			std::cout << "Enter start CS ID: \n";
			startCSID = GetCorrectInput(0, INT_MAX);
			std::cout << "Enter end CS ID: \n";
			endCSID = GetCorrectInput(0, INT_MAX);
			std::cout << "Enter diameter: \n";
			std::cin >> diameter;
			if (!network.ConnectCS(startCSID, endCSID, diameter)) {
				std::cout << "Failed to establish connection.\n";
			}
			break;
		}
		case 6: {
			if (network.getGraphMat().empty())
				std::cout << "There's no added networks!\n";
			else
				network.DisplayNetwork();
			break;
		}
		case 7: {
			network.DeleteGTN();
			std::cout << "Network was successfully deleted\n";
			break;
		}
		case 8: {
			if (network.GetPipePackage().empty() || network.GetCSPackage().empty()) {
				std::cout << "There's nothing to sort!\n";
			}
			else {
				network.TopologicalSort();
			}
			break;
		}
		case 9:
			Save(network.GetPipePackage(), network.GetCSPackage(), network.getGraphMat());
			break;
		case 10:
			Load(network.GetPipePackage(), network.GetCSPackage(), network.getGraphMat());
			break;
		case 11: {
			if (network.getGraphMat().empty()) {
				std::cout << "There's no networks to find the highest flow!\n";
			}
			else {
				int source, sink;
				std::cout << "Enter source CS: \n";
				source = GetCorrectInput(0, INT_MAX); // Получение корректного идентификатора CS
				std::cout << "Enter sink CS: \n";
				sink = GetCorrectInput(0, INT_MAX);

				// Проверка существования узлов
				if (network.GetCSPackage().find(source) == network.GetCSPackage().end()) {
					std::cout << "Source CS with ID " << source << " does not exist.\n";
					break;
				}
				if (network.GetCSPackage().find(sink) == network.GetCSPackage().end()) {
					std::cout << "Sink CS with ID " << sink << " does not exist.\n";
					break;
				}

				// Вызов алгоритма Эдмондса-Карпа
				double maxFlow = network.EdmondsKarp(source, sink);

				// Вывод результатов
				if (maxFlow == 0) {
					std::cout << "No possible flow between source CS " << source << " and sink CS " << sink << ".\n";
				}
				else {
					std::cout << "Maximum flow in the network from CS " << source << " to CS " << sink << " is: " << maxFlow << '\n';
				}
			}
			break;
		}
		case 12: {
			if (network.getGraphMat().empty()) {
				std::cout << "There's no networks to find the shortest path!\n";
			}
			else {
				int source, destination;
				std::cout << "Enter source CS: \n";
				source = GetCorrectInput(0, INT_MAX); // Получение корректного идентификатора CS
				std::cout << "Enter destination CS: \n";
				destination = GetCorrectInput(0, INT_MAX);

				// Проверка существования узлов
				if (network.GetCSPackage().find(source) == network.GetCSPackage().end()) {
					std::cout << "Source CS with ID " << source << " does not exist.\n";
					break;
				}
				if (network.GetCSPackage().find(destination) == network.GetCSPackage().end()) {
					std::cout << "Destination CS with ID " << destination << " does not exist.\n";
					break;
				}

				// Вызов алгоритма Дейкстры
				auto path = network.FindShortestPath(source, destination);

				// Вывод результатов
				if (path.empty()) {
					std::cout << "No path found between CS " << source << " and CS " << destination << ".\n";
				}
				else {
					double totalDistance = 0.0;
					std::cout << "Shortest path from CS " << source << " to CS " << destination << " is: \n";
					for (size_t i = 0; i < path.size(); ++i) {
						std::cout << path[i];
						if (i < path.size() - 1) {
							std::cout << " -> ";
						}
					}
					std::cout << "\n";

					// Расчет общей длины пути
					for (size_t i = 1; i < path.size(); ++i) {
						int from = path[i - 1];
						int to = path[i];
						const Pipe& pipe = network.GetPipePackage().at(network.getGraphMat().at(from).at(to));
						totalDistance += pipe.GetLength();
					}

					std::cout << "Total distance: " << totalDistance << " meters.\n";
				}
			}
			break;
		}
		case 0:
			return;
		}
	}
}

int main() {

	redirect_output_wrapper cerr_out(std::cerr);
	std::string time = std::format("{:%d_%m_%Y %H_%M_%OS}", std::chrono::system_clock::now());
	std::ofstream logfile("log_" + time + ".txt");
	if (logfile)
		cerr_out.redirect(logfile);

	MainMenu();

}