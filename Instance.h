#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Structures.h"
#include "FileReader.h"

#define POP_SIZE 50
#define PROB_CROSSOVER_STATIONARY 1
#define PROB_CROSSOVER_GENERATIONAL 0.7
#define PROB_MUTATION 0.001

class Instance {

private:
	int matrixSize;
	FileReader* matricesFileReader;

	void displaySolution(int* solution);
	
public:
	Instance(std::string path);
	~Instance();

	int * AGEPMX(int * cost /*,std::ofstream &outfile */);
	int * AGGPMX(int * cost /*,std::ofstream &outfile */);
	int evaluateSolution(int* solution);
	static bool compareElements(Element i, Element j);
	bool areElementsEquals(Element firstElement, Element secondElement);

	// Interval : [ intervalBegining ; intervalEnding [
	int* PMXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);
	int* OXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);
};
