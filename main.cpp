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

	int cost = 0;

	std::stringstream path;
	path << "datos/" << argv[1];
	Instance* instance = new Instance(path.str());
	std::srand(atoi(argv[2]));

	// AGE PMX
	std::ofstream outfile;

	//outfile.open("logs/BF" + argv[1] + ".txt");

	const clock_t bestFirst_begin_time = clock();
	instance->AGEPMX(&cost /*, outfile*/);
	std::cout << "Best First execution time: " << float(clock() - bestFirst_begin_time) / CLOCKS_PER_SEC << std::endl;
	outfile.close();
}
