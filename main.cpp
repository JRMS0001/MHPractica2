#include <iostream>
#include <sstream>
#include <time.h>
#include "Instance.h"
#include "FileReader.h"
#include "InputsFileReader.h"
#include "Structures.h"

int main(int argc,const char * argv[]) {

	InputsFileReader *ifl= new InputsFileReader();

	std::string path = "";
	path.append("datos/").append(ifl->input);
	Instance* instance = new Instance(path);
	std::srand(ifl->seed);

	// Stationary
	if (ifl->algorithm == AGE) {
		std::cout << "Executing Stationary algorithm." << std::endl;
		int stationaryCost = 0;
		std::ofstream stationaryOutfile;
		outfile.open("logs/AGE" + ifl->input + ifl->seed +".txt");
		const clock_t stationary_begin_time = clock();
		instance->AGE(ifl->crossover, &stationaryCost , stationaryOutfile);
		std::cout << "Stationary execution time: " << float(clock() - stationary_begin_time) / CLOCKS_PER_SEC << std::endl;
		std::cout << "Stationary cost: " << stationaryCost << std::endl;
		stationaryOutfile.close();
	}

	// Generational
	else if (ifl->algorithm == AGG) {
		std::cout << "Executing Stationary algorithm." << std::endl;
		int generationalCost = 0;
		std::ofstream generationalOutfile;
		outfile.open("logs/AGG" + ifl->input + ifl->seed +".txt");
		const clock_t generational_begin_time = clock();
		instance->AGG(ifl->crossover, &generationalCost , generationalOutfile);
		std::cout << "Generational execution time: " << float(clock() - generational_begin_time) / CLOCKS_PER_SEC << std::endl;
		std::cout << "Stationary cost: " << generationalCost << std::endl;
		generationalOutfile.close();
	}

	system("pause");
}
