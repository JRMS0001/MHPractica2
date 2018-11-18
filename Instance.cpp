#include "Instance.h"

std::Instance::Instance(string path) {
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


void std::Instance::display(){
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
bool std::Instance::comparator(Element i, Element j){
	return i.cost<j.cost;
}

int * std::Instance::AGEPMX(int * cost,ofstream &outfile ){
	std::vector<std::Element> population;

	//Generate the random population
	int* unitAndLocationAssociation = new int[matrixSize];
	for (int i=0; i< POP_SIZE; i++){
		*cost=0;
		/*Random solution*/
		for(int j=0; j<matrixSize; j++){

			unitAndLocationAssociation[j]= j+1;
		}
		random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);


		std::Element *el = new std::Element(unitAndLocationAssociation,matrixSize,this);
		el->Evaluate();
		population.push_back(*el);
	}
	int it=1;
	while(it < 50000){

		//Selection of the fathers
			//First binary tournament
		int r =rand() % POP_SIZE;
		int s =rand() % POP_SIZE;

		std::Element firstFather;
		std::Element secondFather;
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

		//Crossover
		std::vector<Element> sons;
		int* son1;
		int* son2;
		int intervalBegining = 2;
		int intervalEnd = 4;

		// TODO : calculate probabilityCrossover (if yes -> crossover -> son, if no -> keep the first parent)
		/*
		son1 = PMXCrossover(father[], mother[], intervalBegining, intervalEnd);
		son2 = PMXCrossover(mother[], father[], intervalBegining, intervalEnd);
		*/
		son1 = PMXCrossover(firstFather.solution, secondFather.solution, intervalBegining, intervalEnd);
		son2 = PMXCrossover(secondFather.solution, firstFather.solution, intervalBegining, intervalEnd);

		//Mutation
		std::Element firstSon=sons.at(0);
		std::Element secondSon=sons.at(1);
		for(int i=0; i< POP_SIZE; i++){
			for (int j=0; j< matrixSize;j++){
				double random=rand() / (double) RAND_MAX; // between 0 and 1
				if(random < 0.001*(double)matrixSize){
					random = rand() % matrixSize;
					//Swapping elements in the solution
					int swap  = population.at(i).solution[j];
					population.at(i).solution[j]=  population.at(i).solution[(int)random];
					population.at(i).solution[(int)random]=swap;
					//Factorization
					if(population.at(i).eval==false){
						population.at(i).Evaluate();
					}else{
						int mutationCost = population.at(i).cost;

						for(int k=0;k<matrixSize;k++){
							if(k!=j && k!=random){
								mutationCost+=flowMatrix[j][k] * (distanceMatrix[population.at(i).solution[(int)random]-1][population.at(i).solution[k]-1] - distanceMatrix[population.at(i).solution[j]-1][population.at(i).solution[k]-1]);
								mutationCost+=flowMatrix[(int)random][k] * (distanceMatrix[population.at(i).solution[j]-1][population.at(i).solution[k]-1] - distanceMatrix[population.at(i).solution[(int)random]-1][population.at(i).solution[k]-1]);
								mutationCost+=flowMatrix[k][j] * (distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[(int)random]-1] - distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[j]-1]);
								mutationCost+=flowMatrix[k][(int)random] * (distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[j]-1] - distanceMatrix[population.at(i).solution[k]-1][population.at(i).solution[(int)random]-1]);
							}
						}
						population.at(i).cost = mutationCost;
						population.at(i).eval=true;
					}
					it++;
				}
			}
		}

		//Replacement
		std::sort (population.begin(),population.end(),comparator);
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
	//Return the best element
	std::sort (population.begin(),population.end(), comparator);
	*cost= population.at(0).cost;
	return population.at(0).solution;

}


int* std::Instance::OXCrossover(int* father, int* mother, int intervalBegining, int intervalEnd) {

	int intervalSize = intervalEnd - intervalBegining;
	if (1 < intervalSize && intervalSize < matrixSize - 2) {

		/* VARIABLES */
		int* son;
		std::vector<int> fatherIntervalValues;
		std::vector<int> motherSortedValues;
		bool* motherMask = new bool[matrixSize];
		// Initializing mother mask
		for (int i = 0; i < matrixSize; i++) {
			motherMask[i] = 0;
		}

		/* VALUES FROM FATHER TO SON */
		for (int i = intervalBegining; i < intervalEnd; i++) {
			son[i] = father[i];
			fatherIntervalValues.push_back(father[i]);
		}

		/* VALUES FROM MOTHER TO SON */

		// Fill in mother mask
		for (int value : fatherIntervalValues) {
			for (int i = 0; i < matrixSize; i++) {
				if (value == mother[i]) {
					motherMask[i] = 1;
					break;
				}
			}
		}

		// Taking (and sorting) values from mother
		for (int i = intervalEnd; i < matrixSize; i++) {
			if (motherMask[i] == 0) {
				motherSortedValues.push_back(mother[i]);
			}
		}
		for (int i = 0; i < intervalEnd; i++) {
			if (motherMask[i] == 0) {
				motherSortedValues.push_back(mother[i]);
			}
		}

		// Fill in son
		int iterationCounter = -1;
		for (int i = intervalEnd; i < matrixSize; i++) {
			son[i] = motherSortedValues[iterationCounter];
			iterationCounter++;
		}
		for (int i = 0; i < intervalBegining; i++) {
			son[i] = motherSortedValues[iterationCounter];
			iterationCounter++;
		}
		
		return son;

	}
	else {
		return NULL;
	}
}

int* std::Instance::PMXCrossover(int* father, int* mother, int intervalBegining, int intervalEnd){

	int intervalSize = intervalEnd - intervalBegining;
	if (1 < intervalSize && intervalSize < matrixSize - 2) {

		/* VARIABLES */
		int* son;
		bool* motherMask = new bool[matrixSize];
		// Initializing mother mask
		for (int i = 0; i < matrixSize; i++) {
			motherMask[i] = 0;
		}

		/* VALUES COPIED AT THE SAME PLACE FROM FATHER TO SON */
		for (int i = intervalBegining; i < intervalEnd; i++) {
			son[i] = father[i];
		}

		/* VALUES COPIED FROM MOTHER TO SON */

		// Outside the father interval (on the left)
		for (int indexFather = 0; indexFather < intervalBegining; indexFather++) {
			// Outside the mother interval (on the left)
			for (int indexMother = 0; indexMother < intervalBegining; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] == mother[indexMother];
					break;
				}
			}
			// Outside the mother interval (on the right)
			for (int indexMother = intervalEnd; indexMother < matrixSize; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] == mother[indexMother];
					break;
				}
			}
			// Inside the mother interval
			for (int indexMother = intervalBegining; indexMother < intervalEnd; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					int indexSon = indexMother;
					while (intervalBegining <= indexSon && indexSon < intervalEnd) {
						for (int i = 0; i < matrixSize; i++) {
							if (father[indexSon] == mother[i]) {
								indexSon = i;
							}
						}
					}
					son[indexSon] = mother[indexMother];
					break;
				}
			}
		}
		// Outside the father interval (on the right)
		for (int indexFather = intervalEnd; indexFather < matrixSize; indexFather++) {
			// Outside the mother interval (on the left)
			for (int indexMother = 0; indexMother < intervalBegining; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] == mother[indexMother];
					break;
				}
			}
			// Outside the mother interval (on the right)
			for (int indexMother = intervalEnd; indexMother < matrixSize; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] == mother[indexMother];
					break;
				}
			}
			// Inside the mother interval
			for (int indexMother = intervalBegining; indexMother < intervalEnd; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					int indexSon = indexMother;
					while (intervalBegining <= indexSon && indexSon < intervalEnd) {
						for (int i = 0; i < matrixSize; i++) {
							if (father[indexSon] == mother[i]) {
								indexSon = i;
							}
						}
					}
					son[indexSon] = mother[indexMother];
					break;
				}
			}
		}

		return son;

	}
	else {
		return NULL;
	}
}


std::Instance::~Instance() {
	// TODO Auto-generated destructor stub
}

size_t std::Instance::split(const std::string &txt, std::vector<std::string> &strs, char ch)
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
