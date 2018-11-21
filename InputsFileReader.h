#pragma once

#include <fstream>
#include <string>
#include <vector>

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
	float probabilityMutation;
	int eliteNumber;

private:
	void readInputsFromDataFile(std::string path);
	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
};
