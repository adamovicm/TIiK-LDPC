#ifndef DEKODER
#define DEKODER

#include <vector>

using namespace std;

void nadjiProvere(vector<vector<int>>&, vector<vector<int>>&);
void nadjiBite(vector<vector<int>>&, vector<vector<int>>&);
void inicExtrinsic(vector<vector<double>>&, vector<vector<int>>&);

void XuC(vector<double>&, vector<vector<double>>&, vector<vector<int>>&, vector<vector<double>>&);
void proveraParnosti(vector<vector<double>>&, vector<int>&, int);
void MinSum(vector<vector<double>>&, vector<vector<int>>&, vector<vector<double>>&);
void getAposteriori(vector<double>&, vector<double>&, vector<vector<double>>&);
//void provera(vector<double>&, vector<double>&, vector<vector<int>>&, vector<vector<double>>&, int);
void provera(vector<double>&, vector<double>&, vector<vector<int>>&, vector<vector<double>>&, vector<int>&, int);

#endif

