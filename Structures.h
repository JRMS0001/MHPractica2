#pragma once

struct Element {
	int* solution = nullptr;
	int cost = NULL;
};

enum ALGORITHM {
	AGE,
	AGG
};

enum CROSSOVER {
	PMX,
	OX
};