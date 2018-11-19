#include "FileReader.h"

class Element {

public:
	int * solution;
	int cost;
	int size;
	bool eval;

	Element();
	Element(int* solution,int size);
	~Element();
	void Evaluate();
};
