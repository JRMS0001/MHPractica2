/*
 * Instance.h
 *
 *  Created on: Oct 1, 2018
 *      Author: melthalas
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Element.h>
#define POP_SIZE 50

using namespace std;

class Instance {

	int matrixSize;
	int** flowMatrix;
	int** distanceMatrix;
	bool comparator(Element i, Element j);
public:
	Instance(string path);
	void display();
	int * AGEPMX(int * cost,ofstream &outfile );
	std::vector<Element> PMXCrossover(Element first, Element second);
	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
	virtual ~Instance();
};

#endif /* INSTANCE_H_ */
