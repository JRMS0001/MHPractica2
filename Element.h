/*
 * Element.h
 *
 *  Created on: Nov 11, 2018
 *      Author: melthalas
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <Instance.h>

namespace std {

class Element {
	int * solution;
	int cost;
	int size;
	bool eval;
	Instance ins;
public:
	Element(int* solution,int size, Instance ins);
	virtual ~Element();
	void Evaluate();
};

} /* namespace std */

#endif /* ELEMENT_H_ */
