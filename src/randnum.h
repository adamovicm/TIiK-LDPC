#ifndef RANDNUM
#define RANDNUM

#include <array>
#include <windows.h>
#include <random>

using namespace std;


struct RandomData
{
	/****************************
	d1, d2 - kola za kasnjenje
    c - 2/3 epsilon
    a1, a2 - mnozaci
    ra - niz nasumicnih brojeva
    *****************************/
	double d1, d2, c;
	int a1, a2;
    array<double,128> ra;
};

double rng(RandomData&);
double get_rn(RandomData&);
int bin(double);
double gaus(RandomData&);

#endif
