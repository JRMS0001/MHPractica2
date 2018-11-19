#include <iostream>
#include <time.h>
#include "Instance.h"
#include "FileReader.h"
using namespace std;

int main(int argc,const char * argv[]) {

	// Temporary //
	argv[1] = "datos/cnf01.dat";
	argv[2] = "77374129";
	// -------- //

	cout << "File:" << argv[1] << endl;
	cout << "Seed:" << argv[2] << endl;

	int cost = 0;

	Instance* instance = new Instance(argv[1]);
	std::srand(atoi(argv[2]));

	// AGE PMX
	ofstream outfile;
	std::vector<string> v;
	FileReader::getInstance()->split(argv[1],v,'/');
	outfile.open("logs/BF" + v.at(1) + argv[2] + ".txt");
	const clock_t bestFirst_begin_time = clock();
	instance->AGEPMX(&cost, outfile);
	std::cout << "Best First execution time: " << float(clock() - bestFirst_begin_time) / CLOCKS_PER_SEC << endl;
	outfile.close();
}
