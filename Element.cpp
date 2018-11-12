/*
 * Element.cpp
 *
 *  Created on: Nov 11, 2018
 *      Author: melthalas
 */

#include "Element.h"

namespace std {

Element::Element(int* solution,int size, Instance ins) {
	this->cost = 0;
	this->size = size;
	this->solution= new int[size];
	for(int i=0; i<size; i++){
		this->solution[i]= solution[i];
	}
	eval=false;
	this->ins=ins;

}

void Element::Evaluate(){
	//Calculation of the cost of the generated solution
	cost=0;
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			if(i!=j)
				cost+=ins.flowMatrix[i][j] * ins.distanceMatrix[solution[i]-1][solution[j]-1];
		}
	}
	eval=true;
}

Element::~Element() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
