#include "Element.h"

Element::Element(){
	size=0;
	cost=0;
	this->solution=NULL;
	eval=false;
}

Element::Element(int* solution,int size) {
	this->cost = 0;
	this->size = size;
	this->solution= new int[size];
	for(int i=0; i<size; i++){
		this->solution[i]= solution[i];
	}
	eval=false;
}

void Element::Evaluate(){
	//Calculation of the cost of the generated solution
	int** flowMatrix = FileReader::getInstance()->getFlowMatrix();
	int** distanceMatrix = FileReader::getInstance()->getDistanceMatrix();

	cost=0;
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if(i!=j)
				cost += flowMatrix[i][j] * distanceMatrix[solution[i]-1][solution[j]-1];
		}
	}
	eval=true;
}

Element::~Element() {
	// TODO Auto-generated destructor stub
}
