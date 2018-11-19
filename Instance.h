#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Element.h"
#include "FileReader.h"
#define POP_SIZE 50

class Element;

class Instance {

private:
	int** flowMatrix;
	int** distanceMatrix;
	int matrixSize;
	
public:
	Instance(std::string path);
	~Instance();

	int * AGEPMX(int * cost,std::ofstream &outfile );
	static bool comparator(Element i, Element j);

	// Interval : [ intervalBegining ; intervalEnding [
	int* PMXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);
	int* OXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);
};
