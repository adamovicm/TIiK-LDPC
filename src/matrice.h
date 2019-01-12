#ifndef MATRICE
#define MATRICE

#include <vector>
#include <fstream>
#include <string>

using namespace std;

/***** FUNKCIJE ZA PRAVLJENJE MATRICE *****/
void inicTraku(vector<vector<bool>>&, const int&, const int&, const int&);
void shuffle(vector<vector<bool>>&);
void makeMatrix(vector<vector<bool>>&, vector<vector<bool>>&, int);

/***** I/O *****/
void txt2matrix(string, vector<vector<int>>&);
void mat2txt(ofstream&, vector<vector<bool>>&);

/***** FUNKCIJE ZA GAUSOVU ELIMINACIJU *****/
bool findOneAbove(vector<vector<bool>>&, int, int);
bool findOneLeft(vector<vector<bool>>&, vector<vector<bool>>&, int, int);
bool findOneInRect(vector<vector<bool>>&, vector<vector<bool>>&, int, int);
void removeOtherOnes(vector<vector<bool>>&, int, int);
void gaussElimination(vector<vector<bool>>&, vector<vector<bool>>&);

#endif
