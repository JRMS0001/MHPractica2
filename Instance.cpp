#include "Instance.h"




/* CONSTRUCTOR AND DESTRUCTOR */

Instance::Instance(std::string path) {
	FileReader::getInstance()->readMatrixFromDataFile(path);
	flowMatrix = FileReader::getInstance()->getFlowMatrix();
	distanceMatrix = FileReader::getInstance()->getDistanceMatrix();
	matrixSize = FileReader::getInstance()->getMatrixSize();
}

Instance::~Instance() {
}




/* GENETICAL ALGORITHMS */

int* Instance::AGEPMX(int * cost, std::ofstream &outfile ){
	std::vector<Element> population;

	// Generate the random population
	int* unitAndLocationAssociation = new int[matrixSize];
	for (int i=0; i< POP_SIZE; i++){
		*cost=0;
		/*Random solution*/
		for(int j=0; j<matrixSize; j++){
			unitAndLocationAssociation[j]= j+1;
		}
		std::random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);

		Element element;
		element.solution = unitAndLocationAssociation;
		element.cost = evaluateSolution(unitAndLocationAssociation);

		population.push_back(element);
	}



	int it=1;
	while(it < 50000){



		/* SELECTION */

		//First binary tournament
		int r = rand() % POP_SIZE;
		int s = rand() % POP_SIZE;
		Element firstFather;
		Element secondFather;
		if(population.at(r).cost < population.at(s).cost ){
			firstFather = population.at(r);
		}
		else{
			firstFather = population.at(s);
		}

		//Second binary tournament
		r = rand() % POP_SIZE;
		s = rand() % POP_SIZE;
		if(population.at(r).cost < population.at(s).cost ){
			secondFather = population.at(r);
		}
		else{
			secondFather = population.at(s);
		}



		/* CROSSOVER */

		int* solutionSon1;
		int* solutionSon2;
		int intervalBegining = 2;
		int intervalEnd = 4;

		double pCrossover = rand() / (double)RAND_MAX; // between 0 and 1
		if (pCrossover <= PROB_CROSSOVER_STATIONARY) {
			solutionSon1 = PMXCrossover(firstFather.solution, secondFather.solution, intervalBegining, intervalEnd);
			solutionSon2 = PMXCrossover(secondFather.solution, firstFather.solution, intervalBegining, intervalEnd);
		}
		else {
			solutionSon1 = firstFather.solution;
			solutionSon2 = secondFather.solution;
		}

		Element firstSon;
		firstSon.solution = solutionSon1;
		firstSon.cost = evaluateSolution(solutionSon1);

		Element secondSon;
		secondSon.solution = solutionSon2;
		secondSon.cost = evaluateSolution(solutionSon2);



		/* MUTATION */

		for(int i=0; i< POP_SIZE; i++){
			for (int j=0; j< matrixSize;j++){
				double random = rand() / (double)RAND_MAX; // between 0 and 1
				if(random < PROB_MUTATION * (double)matrixSize){
					random = rand() % matrixSize;
					//Swapping elements in the solution
					int swap  = population.at(i).solution[j];
					population.at(i).solution[j]=  population.at(i).solution[(int)random];
					population.at(i).solution[(int)random]=swap;
					//Factorization
					if (population.at(i).cost == NULL) {
						population.at(i).cost = evaluateSolution(population.at(i).solution);
					}
					else {
						int mutationCost = population.at(i).cost;
						for (int k = 0; k < matrixSize; k++) {
							if (k != j && k != random) {
								mutationCost += flowMatrix[j][k] * (distanceMatrix[population.at(i).solution[(int)random] - 1][population.at(i).solution[k] - 1] - distanceMatrix[population.at(i).solution[j] - 1][population.at(i).solution[k] - 1]);
								mutationCost += flowMatrix[(int)random][k] * (distanceMatrix[population.at(i).solution[j] - 1][population.at(i).solution[k] - 1] - distanceMatrix[population.at(i).solution[(int)random] - 1][population.at(i).solution[k] - 1]);
								mutationCost += flowMatrix[k][j] * (distanceMatrix[population.at(i).solution[k] - 1][population.at(i).solution[(int)random] - 1] - distanceMatrix[population.at(i).solution[k] - 1][population.at(i).solution[j] - 1]);
								mutationCost += flowMatrix[k][(int)random] * (distanceMatrix[population.at(i).solution[k] - 1][population.at(i).solution[j] - 1] - distanceMatrix[population.at(i).solution[k] - 1][population.at(i).solution[(int)random] - 1]);
							}
						}
						population.at(i).cost = mutationCost;
					}
					it++;
				}
			}
		}



		/* REPLACEMENT */

		std::sort(population.begin(),population.end(), &compareElements);
		Element worst1 = population.at(POP_SIZE-2);
		Element worst2 = population.at(POP_SIZE-1);
		if(firstSon.cost< worst2.cost){
			if(firstSon.cost < worst1.cost){
				population.at(POP_SIZE-2)= firstSon;
			}else{
				population.at(POP_SIZE-1)= firstSon;
			}
		}
		std::sort (population.begin(),population.end(), &compareElements);
		worst1 = population.at(POP_SIZE-2);
		worst2 = population.at(POP_SIZE-1);
		if(secondSon.cost< worst2.cost){
			if(secondSon.cost < worst1.cost){
				population.at(POP_SIZE-2) = secondSon;
			}else{
				population.at(POP_SIZE-1) = secondSon;
			}
		}
	}



	/* RETURN */
	std::sort(population.begin(),population.end(), &compareElements);
	*cost= population.at(0).cost;
	return population.at(0).solution;
}




/* USEFUL FUNCTIONS */

int Instance::evaluateSolution(int* solution) {
	//Calculation of the cost of the generated solution
	int cost = 0;
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			if (i != j)
				cost += flowMatrix[i][j] * distanceMatrix[solution[i] - 1][solution[j] - 1];
		}
	}
}

bool Instance::compareElements(Element i, Element j) {
	return i.cost < j.cost;
}




/* OX AND PMX CROSSOVERS */

int* Instance::OXCrossover(int* father, int* mother, int intervalBegining, int intervalEnd) {

	int intervalSize = intervalEnd - intervalBegining;
	if (1 < intervalSize && intervalSize < matrixSize - 2) {

		/* VARIABLES */
		int *son = new int[matrixSize];
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
		std::cout << "WARNING! The crossover interval is not correct." << std::endl;
		return NULL;
	}
}

int* Instance::PMXCrossover(int* father, int* mother, int intervalBegining, int intervalEnd){

	int intervalSize = intervalEnd - intervalBegining;
	if (1 < intervalSize && intervalSize < matrixSize - 2) {

		/* VARIABLES */
		int *son = new int[matrixSize];
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
					son[indexMother] = mother[indexMother];
					break;
				}
			}
			// Outside the mother interval (on the right)
			for (int indexMother = intervalEnd; indexMother < matrixSize; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] = mother[indexMother];
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
					son[indexMother] = mother[indexMother];
					break;
				}
			}
			// Outside the mother interval (on the right)
			for (int indexMother = intervalEnd; indexMother < matrixSize; indexMother++) {
				if (father[indexFather] == mother[indexMother]) {
					son[indexMother] = mother[indexMother];
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
		std::cout << "WARNING! The crossover interval is not correct." << std::endl;
		return NULL;
	}
}
