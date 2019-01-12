#include "statistika.h"
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;


void findRhoFER(vector<vector<int>>& fer, vector<double>& StatFER, vector<double>& Ro)
{
	double suma, suma2, Ex, DEx;
    int n(fer.size());

    for(unsigned int iter = 0; iter < fer[0].size(); ++iter)
	{
		suma = 0;
		suma2 = 0;
		for(int sekv = 0; sekv < n; ++sekv)
		{
			suma += fer[sekv][iter];
			suma2 += fer[sekv][iter] * fer[sekv][iter];
		}
		Ex = suma / n;
		StatFER[iter] = Ex;
		DEx = suma2 / (n * (n-1))  -  suma * suma / (n * n * (n - 1));
		Ro[iter] = sqrt(DEx) / Ex;
	}
}

void findRhoBER(vector<vector<double>>& ber, vector<double>& StatBER, vector<double>& Ro)
{
	double suma(0), suma2(0), Ex, DEx;
    int n(ber.size());

    //double tmp;

    for(unsigned int iter = 0; iter < ber[0].size(); ++iter)
	{
		suma = 0;
		suma2 = 0;
		for(int sekv = 0; sekv < n; ++sekv)
		{
			suma += ber[sekv][iter];
			suma2 += ber[sekv][iter] * ber[sekv][iter];
		}
		Ex = suma / n;
		StatBER[iter] = Ex;
		DEx = suma2 / (n * (n-1))  -  suma * suma / (n * n * (n - 1));
		Ro[iter] = sqrt(DEx) / Ex;
	}
}

void writeFER(vector<double>& StatFER, double snr)  /// 0 - dobro, 1 - greska u frejmu
{
	ofstream f;
	f.open("FER.txt", fstream::out | fstream::app);
	if(f.is_open())
	{
		f << snr << " "; /// upisujemo prvo vrednost trenutnog SNR
		for(unsigned int iter = 0; iter < StatFER.size(); ++iter)
		{
			f << StatFER[iter] << " ";
		}
		f << endl;
		f.close();
	}
	else cout << "\n\nGRESKA PRI OTVARANJU FER.txt FAJLA!!!\n\n";
}

void writeBER(vector<double>& StatBER, double snr)  /// 0 - dobro, 1 - greska u frejmu
{
	ofstream f;
	f.open("BER.txt", fstream::out | fstream::app);
	if(f.is_open())
	{
		f << snr << " "; /// upisujemo prvo vrednost trenutnog SNR
		for(unsigned int iter = 0; iter < StatBER.size(); ++iter)
		{
			f << StatBER[iter] << " ";
		}
		f << endl;
		f.close();
	}
	else cout << "\n\nGRESKA PRI OTVARANJU BER.txt FAJLA!!!\n\n";
}
