#ifndef STATISTIKA
#define STATISTIKA

//#include <fstream>
#include <vector>

using namespace std;


//template <class T>
//void findRho(vector<vector<T>>&, vector<double>&);
//void findRhos();
//void findRho(vector<vector<auto>>&, vector<double>&);
void findRhoFER(vector<vector<int>>&, vector<double>&, vector<double>&);
void findRhoBER(vector<vector<double>>&, vector<double>&, vector<double>&);

//bool newSNR(double);

void writeFER(vector<double>&, double); /// 0 - dobro, 1 - greska u frejmu
void writeBER(vector<double>&, double); /// vrednosti izmedju 0 i 1, u zavisnosti od kolicine gresaka


bool razlike(vector<int>&, vector<double>&);


#endif // STATISTIKA
