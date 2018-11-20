#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class FileReader {
public:
	static FileReader *getInstance();
	void readMatrixFromDataFile(std::string path);

	int getMatrixSize();
	int** getFlowMatrix();
	int** getDistanceMatrix();

	~FileReader();
	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
	void displayMatrices();

private:
	FileReader();
	static FileReader *instance;
	
	int matrixSize;
	int** flowMatrix;
	int** distanceMatrix;
};

