#include <iostream>
#include <sstream>
#include <time.h>
#include "Instance.h"
#include "FileReader.h"
#include "InputsFileReader.h"

int main(int argc,const char * argv[]) {

	// Temporary //
	//argv[1] = "cnf01.dat";
	//argv[2] = "77374129";
	// -------- //
	InputsFileReader *ifl= new InputsFileReader();

	std::cout << "File:" << ifl->input << std::endl;
	std::cout << "Seed:" << ifl->seed << std::endl;


	std::string path = "";
	path.append("datos/").append(ifl->input);
	Instance* instance = new Instance(path);
	std::srand(ifl->seed);


	// Stationary PMX
	int stationaryCost = 0;
	std::ofstream stationaryOutfile;
	//outfile.open("logs/BF" + argv[1] + ".txt");
	const clock_t stationary_begin_time = clock();
	//instance->AGEPMX(&stationaryCost /*, stationaryOutfile*/);
	std::cout << "Stationary execution time: " << float(clock() - stationary_begin_time) / CLOCKS_PER_SEC << std::endl;
	std::cout << "Stationary cost: " << stationaryCost << std::endl;
	stationaryOutfile.close();

	// Generational PMX
	int generationalCost = 0;
	std::ofstream generationalOutfile;
	//outfile.open("logs/BF" + argv[1] + ".txt");
	const clock_t generational_begin_time = clock();
	instance->AGGPMX(&generationalCost /*, generationalOutfile*/);
	std::cout << "Generational execution time: " << float(clock() - generational_begin_time) / CLOCKS_PER_SEC << std::endl;
	generationalOutfile.close();

	system("pause");
}
