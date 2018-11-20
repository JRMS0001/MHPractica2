#pragma once

#include <string>

#define INPUTS_FILE_PATH "InputsFile.txt"

class InputsFileReader
{
public:
	InputsFileReader();
	~InputsFileReader();

	std::string algorithm;
	std::string input;
	std::string outputLog;
	std::string outputRes;
	int seed;
	int populationSize;
	int generationsMax;
	float probabilityCrossover;
	float probabilityMutation = 0.001;
	int eliteNumber;

private:
	void readInputsFromDataFile(std::string path);
};
