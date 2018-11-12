#include <iostream>
#include "Instance.h"
using namespace std;

int main(int argc,const char * argv[]) {
	argc = 11;
	argv[1] = (char*)"datos/cnf01.dat";
	argv[2] = (char*)"datos/cnf02.dat";
	argv[3] = (char*)"datos/cnf03.dat";
	argv[4] = (char*)"datos/cnf04dat.sec";
	argv[5] = (char*)"datos/cnf05dat.sec";
	argv[6] = (char*)"datos/cnf06.dat";
	argv[7] = (char*)"datos/cnf07.dat";
	argv[8] = (char*)"datos/cnf08.dat";
	argv[9] = (char*)"datos/cnf09.dat";
	argv[10] = (char*)"datos/cnf10.dat";

	int * cost = new int[1];

	std::vector<const char*> seeds;
	seeds.push_back("77374129");
	seeds.push_back("73741297");
	seeds.push_back("37412977");
	seeds.push_back("74129773");
	seeds.push_back("41297737");

	for (int itFile = 1; itFile <= argc - 1; itFile++) {
		cout << "File:" << argv[itFile] << endl;
		Instance* instance = new Instance(argv[itFile]);

		for (int i = 0; i < seeds.size(); i++) {
			cout << "Seeds:" << seeds[i] << endl;
			std::srand(atoi(seeds[i]));
			// AGE PMX
			ofstream outfile;
			std::vector<string> v;
			instance->split(argv[itFile],v,'/');
			outfile.open("logs/BF" + v.at(1) + seeds[i] + ".txt");
			const clock_t bestFirst_begin_time = clock();
			instance->AGEPMX(cost, outfile);
			std::cout << "Best First execution time: " << float(clock() - bestFirst_begin_time) / CLOCKS_PER_SEC << endl;
			outfile.close();

		/*// Simulated Annealing
		instance->split(argv[itFile],v,'/');
		outfile.open("logs/SAB" + v.at(1) + seeds[i] + ".txt");
		const clock_t simulatedAnnealingBoltzmann_begin_time = clock();
		instance->simAnnealingBoltzmann(cost, outfile);
		std::cout << "Simulated Annealing Boltzmann execution time: " << float(clock() - simulatedAnnealingBoltzmann_begin_time) / CLOCKS_PER_SEC << endl;
		outfile.close();
		saBoltzmannAlgorithmValues[i] = *cost;

		instance->split(argv[itFile],v,'/');
		outfile.open("logs/SAG" + v.at(1) + seeds[i] + ".txt");
		const clock_t simulatedAnnealingGeometric_begin_time = clock();
		instance->simAnnealingGeometric(cost, outfile);
		std::cout << "Simulated Annealing Geometric execution time: " << float(clock() - simulatedAnnealingGeometric_begin_time) / CLOCKS_PER_SEC << endl;
		outfile.close();
		saGeometricAlgorithmValues[i] = *cost;
		cout << endl;
*/
		}
	}
}
