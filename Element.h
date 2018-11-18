#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "Instance.h"


namespace std {
class Instance;
class Element {

public:
	int * solution;
	int cost;
	int size;
	bool eval;
	Instance *ins;

	Element();
	Element(int* solution,int size, std::Instance *ins);
	virtual ~Element();
	void Evaluate();
};

} /* namespace std */

#endif /* ELEMENT_H_ */
