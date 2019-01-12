#include "pomocne.h"
#include <iostream>
#include <fstream>
#include <iso646.h>

using namespace std;


void stanje(vector<double>& kodnaRec, vector<double>& lambda)
{
	/*
	cout << "\n\nKodna Rec\n\n";
    for(unsigned int x = 0; x < kodnaRec.size(); ++x)
	{
		cout << kodnaRec[x] << " ";
	}

	cout << "\n\nLambda\n\n";
    for(unsigned int x = 0; x < lambda.size(); ++x)
	{
		cout << lambda[x] << " ";
	}

	cout << endl;
	*/
	int brojacRazlika(0);
	for(unsigned int i = 0; i < kodnaRec.size(); ++i)
	{
		if(((kodnaRec[i] == 1) && (lambda[i] < 0)) || ((kodnaRec[i] == -1) && (lambda[i] >= 0)))
		{
			cout << "\nError at "<< i+1 << ":   " << kodnaRec[i] << "   " << lambda[i];
			++brojacRazlika;
		}
	}
	cout << "\n\nUkupno razlika: " << brojacRazlika << endl;
}

void ispisiRO(vector<double>& RO)
{
	for(unsigned int i = 0; i < RO.size(); ++i)
	{
		cout << RO[i] << "  ";
	}
	cout << endl << endl;
}






template <typename T>
void ispisiER(vector<vector<T>> &ER)
{
	for(int i = 0; i < ER[0].size(); ++i)
	{
		for(int j = 0; j < ER.size(); ++j)
		{
			cout << ER[j] << "  ";
		}
		cout << endl << endl;
	}
}


void ispisiBERove(vector<vector<double>>& ber, vector<double>& berovi, double SNR)
{
	double suma(0), suma2(0), DEx;
	int n(ber.size());
	ofstream f;
	//double tmp;
	
	for (unsigned int iter = 0; iter < ber[0].size(); ++iter)
	{
		suma = 0;
		suma2 = 0;
		for (int sekv = 0; sekv < n; ++sekv)
		{
			suma += ber[sekv][iter];
			suma2 += ber[sekv][iter] * ber[sekv][iter];
		}
		DEx = suma2 / (n * (n - 1)) - suma * suma / (n * n * (n - 1));
		berovi[iter] = DEx;
	}
	f.open("BERovi.txt", fstream::out | fstream::app);
	if (f.is_open())
	{
		f << SNR << " ";
		for (unsigned int i = 0; i < berovi.size(); ++i)
		{
			f << berovi[i] << " ";
		}
		f << endl;
		f.close();
	}
	else cout << "\n\nGRESKA PRI OTVARANJU BERova!!!\n\n";
}

void napraviKodnuRec(vector<double>& kodnaRec, vector<int>& infSekvenca, vector<vector<int>>& gen)
{
	kodnaRec.clear();
	for (unsigned int i = 0; i < gen[0].size(); ++i)
	{
		bool tmp(false);
		for (unsigned int j = 0; j < gen.size(); ++j)
		{
			tmp = tmp xor (bool)(infSekvenca[j] * gen[j][i]);
		}
		if (tmp == false) kodnaRec.push_back(0.0);
		else kodnaRec.push_back(1.0);
	}
}

bool proveriKodnuRec(vector<double>& kodnaRec, vector<vector<int>>& matrica)
{
	bool suma, suma2(false);
	double proizvod;
	vector<bool> tmp(matrica.size());
	for (unsigned int j = 0; j < matrica.size(); ++j)
	{
		suma = 0;
		for (unsigned int i = 0; i < matrica[0].size(); ++i)
		{
			proizvod = matrica[j][i] * kodnaRec[i];
			if(proizvod == 0) suma = suma xor 0;
			else suma = suma xor 1;
		}
		tmp[j] = suma;
		suma2 = suma2 xor suma;
	}
	if (suma2 == false) return true;
	else return false;
}

bool nulaIliJedan(vector<vector<int>>& mat, unsigned int y, unsigned int x)
{
	if (mat[y][x] == 0) return 0;
	else return 1;
}

bool proveriMatrice(vector<vector<int>>& H, vector<vector<int>>& gen)
{
	bool suma, rezultat (true);
	vector<vector<int>> proizvod(H.size(), vector<int>(gen.size()));

	for (unsigned int red = 0; red < H.size(); ++red)
	{
		for (unsigned int kolona = 0; kolona < gen.size(); ++kolona)
		{
			suma = false;
			for (unsigned int x = 0; x < H[0].size(); ++x)
			{
				suma = suma xor (nulaIliJedan(H, red, x) * nulaIliJedan(gen, kolona, x));
			}
			proizvod[red][kolona] = suma;
			if (suma == true) rezultat = false;
		}
	}
	return rezultat;
}

