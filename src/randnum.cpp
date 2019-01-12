#include "randnum.h"


double rng(RandomData& rd)
{
	/*****************************************************
	Funkcija vraca nasumicni broj u vrednosti od 0 do 1.
    *****************************************************/

    double rn, s1, s2, x;
    /****************************
    rn - random number (izlaz)
    s1,s2 - sabirci
    x - zbir pre ulaska u frac.
    *****************************/

    s1 = rd.d1 * rd.a1;
    s2 = rd.d2 * rd.a2;
    x = s1 + s2 + rd.c;
    rn = x - floor(x);
    rd.d2 = rd.d1;
    rd.d1 = rn;

    return rn;
}

double get_rn(RandomData& rd)
{
	/**************************************************************************************************
	Funkcija za uzimanje nasumicnog elementa iz niza i zamene tog clana niza novim nasumicnim brojem.
	**************************************************************************************************/

	unsigned int elem, seed;
	double rn;
	LARGE_INTEGER li;
	/**************************************
	elem - nasumicno odabrani element niza
	seed - seed za RNG
	rn - nasumicni broj (izlaz)
	li - veliki int za seed
	**************************************/

	/***** Ugradjena random funkcija *****/
	QueryPerformanceCounter(&li);
    seed = li.LowPart;
	mt19937 eng(seed); // engine: Mersenne Twister 19937 generator
	uniform_int_distribution<int> dist(0, 127); // uniformna distribucija

	elem = dist(eng);
    rn = rd.ra[elem];
    rd.ra[elem] = rng(rd);

    return rn;
}

int bin(double rn)
{
	/**************************************************
	Funkcija pravi binarnu raspodelu na osnovu ulaza:
	rn >= 0.5  ->  1
	rn <  0.5  ->  0
	***************************************************/

	if(rn >= 0.5) return 1;
	else return 0;
}

double gaus(RandomData& rd)
{
	/****************************************************
	Funkcija vraca nasumicni broj sa Gausovom raspodelom
	****************************************************/

	double u1, u2, m, z;

	do
	{
		u1 = 2.0 * get_rn(rd) - 1.0;
		u2 = 2.0 * get_rn(rd) - 1.0;
		m = u1 * u1 + u2 * u2;
	}while(m >= 1.0 || m == 0);

	z = sqrt((-2.0 * log(m)) / m);

	return u1 * z;
}

