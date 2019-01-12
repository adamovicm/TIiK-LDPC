#ifndef KODER
#define KODER

#include <vector>
#include "randnum.h"

using namespace std;

/************************** Funkcije za koder **************************/
void makeInfSekvence(vector<int>&, vector<vector<int>>&, RandomData&);
void coder(vector<double>&, vector<int>&, vector<vector<int>>&);

/************************** Funkcije za kanal **************************/
void channelTransfer(vector<double>&, vector<double>&, double, RandomData&);

#endif // KODER
