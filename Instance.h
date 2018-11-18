#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Element.h"
#define POP_SIZE 50

class Element;
namespace std{

class Instance {

	int matrixSize;
	static bool comparator(Element i, Element j);
public:
	int** flowMatrix;
	int** distanceMatrix;

	Instance();
	Instance(string path);
	void display();
	int * AGEPMX(int * cost,ofstream &outfile );

	// Interval : [ intervalBegining ; intervalEnding [
	int* PMXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);
	int* OXCrossover(int* father, int* mother, int intervalBegining, int intervalEnding);

	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
	virtual ~Instance();
};
}
#endif /* INSTANCE_H_ */
