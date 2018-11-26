#include "InputsFileReader.h"
#include <algorithm>


InputsFileReader::InputsFileReader()
{
	readInputsFromDataFile(INPUTS_FILE_PATH);
}

InputsFileReader::~InputsFileReader()
{
}

void InputsFileReader::readInputsFromDataFile(std::string path) {

	std::string line;
	std::ifstream myfile(path);
	std::vector<std::string> res;

	if (myfile.is_open()){
		getline(myfile, line);
		this->split(line,res,'=');
		if (res.at(1) == "AGE") {
			algorithm = AGE;
		}
		else if (res.at(1) == "AGG") {
			algorithm = AGG;
		}
		else {

		}

		getline(myfile, line);
		this->split(line, res, '=');
		if (res.at(1) == "PMX") {
			crossover = PMX;
		}
		else if (res.at(1) == "OX") {
			crossover = OX;
		}
		else {
			
		}

		getline(myfile, line);
		this->split(line,res,'=');
		input = res.at(1);

		getline(myfile, line);
		this->split(line,res,'=');
		std::string output= res.at(1);
		this->split(output,res,' ');
		this->outputLog=res.at(0);
		this->outputRes=res.at(1);

		getline(myfile, line);
		this->split(line,res,'=');
		seed = stoi(res.at(1));

		getline(myfile, line);
		this->split(line,res,'=');
		populationSize = stoi(res.at(1));

		getline(myfile, line);
		this->split(line,res,'=');
		generationsMax = stoi(res.at(1));

		getline(myfile, line);
		this->split(line,res,'=');
		probabilityCrossover = stof(res.at(1));

		getline(myfile, line);
		this->split(line,res,'=');
		probabilityMutation = stof(res.at(1));

		getline(myfile, line);
		this->split(line,res,'=');
		eliteNumber = stoi(res.at(1));
	}

}

size_t InputsFileReader::split(const std::string &txt, std::vector<std::string> &strs, char ch) {
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


