#ifndef POMOCNE
#define POMOCNE

#include <vector>

using namespace std;


void stanje(vector<double>&, vector<double>&);
void ispisiRO(vector<double>&);

template <class T>
void ispisiER(vector<vector<T>>&);
void ispisiBERove(vector<vector<double>>&, vector<double>&, double);

void napraviKodnuRec(vector<double>&, vector<int>&, vector<vector<int>>&);
bool proveriKodnuRec(vector<double>&, vector<vector<int>>&);

bool nulaIliJedan(vector<vector<int>>&, unsigned int, unsigned int);
bool proveriMatrice(vector<vector<int>>&, vector<vector<int>>&);


#endif // POMOCNE
