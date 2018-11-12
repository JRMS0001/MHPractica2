/*
 * Instance.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: melthalas
 */

#include "Instance.h"

Instance::Instance(string path) {
	string line;
		ifstream myfile(path);
		matrixSize=0;
		if (myfile.is_open())
		{

			// Getting the matrix size
			getline(myfile, line);
			matrixSize = std::stoi(line);

			// Declaring the matrixes
			flowMatrix = new int*[matrixSize]; //col
			for (int i = 0; i < matrixSize; ++i)
				flowMatrix[i] = new int[matrixSize]; //row

			distanceMatrix = new int*[matrixSize]; //col
			for (int i = 0; i < matrixSize; ++i)
				distanceMatrix[i] = new int[matrixSize]; //row

			// Filling the matrixes in
			int lineNumber = 2;
			while (getline(myfile, line))
			{
				// Flow matrix
				if (3 <= lineNumber && lineNumber < 3 + matrixSize) {
					int i = 0;
					do {
						std::vector<std::string> v;
						v.clear();
						split(line, v, ' ');

						// Clearing blank data (due to double spaces)
						std::vector<std::string> valuesFlow;
						valuesFlow.clear();
						for (int j = 0; j < v.size(); j++) {
							if (!(v[j] == "")) {
								valuesFlow.push_back(v[j]);
							}
						}
						for (int j = 0; j < matrixSize; j++) {
							if(valuesFlow.size()!=0)
								flowMatrix[i][j] = std::stoi(valuesFlow[j]);
						}

						// New line
						getline(myfile, line);
						lineNumber++;
						i++;
					} while (i < matrixSize);
				}

				// Distance matrix
				if (4 + matrixSize <= lineNumber && lineNumber < 4 + matrixSize + matrixSize) {
					int i = 0;
					do {
						std::vector<std::string> v;
						v.clear();
						split(line, v, ' ');

						// Clearing blank data (due to double spaces)
						std::vector<std::string> valuesDistances;
						valuesDistances.clear();
						for (int j = 0; j < v.size(); j++) {
							if (!(v[j] == "")) {
								valuesDistances.push_back(v[j]);
							}
						}
						for (int j = 0; j < matrixSize; j++) {
							if(valuesDistances.size()!=0)
								distanceMatrix[i][j] = std::stoi(valuesDistances[j]);
						}

						// New line
						getline(myfile, line);
						lineNumber++;
						i++;
					} while (i < matrixSize);
				}

				lineNumber++;

			}
			myfile.close();

		}
		else{
			cout << "Unable to open file";
		}
}


void Instance::display(){
	if (matrixSize!=0){
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				cout << " " << flowMatrix[i][j];
			}
			cout << endl;
	}

		cout << endl;
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				cout << " " << distanceMatrix[i][j];
			}
			cout << endl;
		}
	}
}
bool comparator(Element i, Element j){
	return i.cost<j.cost;
}

int * Instance::AGEPMX(int * cost,ofstream &outfile ){
	std::vector<Element> population = new std::vector();

	//Generate the random population
	int* unitAndLocationAssociation = new int[matrixSize];
	for (int i=0; i< POP_SIZE; i++){
		*cost=0;
		/*Random solution*/
		for(int j=0; j<matrixSize; j++){

			unitAndLocationAssociation[j]= j+1;
		}
		random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);


		Element el = new Element(unitAndLocationAssociation,matrixSize,this);
		el.Evaluate();
		population.push_back(el);
	}
	int it=1;
	while(it < 50000){

		//Selection of the fathers
			//First binary tournament
		int r =rand() % POP_SIZE;
		int s =rand() % POP_SIZE;

		Element firstFather;
		Element secondFather;
		if(population.at(r).cost < population.at(s).cost ){
			firstFather = population.at(r);
		}
		else{
			firstFather = population.at(s);
		}
		//Second binary tournament
		r =rand() % POP_SIZE;
		s =rand() % POP_SIZE;

		if(population.at(r).cost < population.at(s).cost ){
			secondFather = population.at(r);
		}
		else{
			secondFather = population.at(s);
		}

		std::vector<Element> sons;
		//Crossover
		sons=PMXCrossover(firstFather, secondFather);

		//Mutation
		Element firstSon=sons.at(0);
		Element secondSon=sons.at(1);
		for(int i=0; i< POP_SIZE; i++){
			for (int j=0; j< matrixSize;j++){
				double random=rand() / (double) RAND_MAX;
				if(random < 0.001*(double)matrixSize){
					random = rand() % matrixSize;
					//Swapping elements in the solution
					int swap  = population.at(i).solution[j];
					population.at(i).solution[j]=  population.at(i).solution[random];
					population.at(i).solution[random]=swap;
					//Factorization
					if(population.at(i).eval==false){
						population.at(i).Evaluate();
					}else{
						int mutationCost = population.at(i).cost;

						for(int k=0;k<matrixSize;k++){
								if(k!=i && k!=j){
									mutationCost+=flowMatrix[j][k] * (distanceMatrix[population.at(i).solution[random]-1][population.at(i).solution[k]-1] - distanceMatrix[population.at(i).solution[j]-1][population.at(i).solution[k]-1]);
									mutationCost+=flowMatrix[random][k] * (distanceMatrix[population.at(i).solution[j]-1][population.at(i).solution[k]-1] - distanceMatrix[population.at(i).solution[random]-1][population.at(i).solution[k]-1]);
									mutationCost+=flowMatrix[k][j] * (distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[random]-1] - distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[j]-1]);
									mutationCost+=flowMatrix[k][random] * (distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[j]-1] - distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[random]-1]);
								}
							}
					}
				}
			}
		}

		//Replacement
		std::sort (population.begin(),population.end(), comparator);
		Element worst1 = population.at(POP_SIZE-2);
		Element worst2 = population.at(POP_SIZE-1);
		if(firstSon.cost< worst2.cost){
			if(firstSon.cost < worst1.cost){
				population.at(POP_SIZE-2)= firstSon;
			}else{
				population.at(POP_SIZE-1)= firstSon;
			}
		}
		std::sort (population.begin(),population.end(), comparator);
		worst1 = population.at(POP_SIZE-2);
		worst2 = population.at(POP_SIZE-1);
		if(secondSon.cost< worst2.cost){
			if(secondSon.cost < worst1.cost){
				population.at(POP_SIZE-2)= secondSon;
			}else{
				population.at(POP_SIZE-1)= secondSon;
			}
		}

	}
}

std::vector<Element> Instance::PMXCrossover(Element first, Element second){
	//TODO
	std::vector sons= new std::vector();

	sons.push_back(first);
	sons.push_back(second);

	return sons;
}

Instance::~Instance() {
	// TODO Auto-generated destructor stub
}

size_t Instance::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}
