#include "dekoder.h"
#include <cmath>

using namespace std;

void nadjiProvere(vector<vector<int>>& matrica, vector<vector<int>>& provere)
{
	/*****************************************************************
	Funkcija trazi redni broj provera M u kojima ucestvuju N-ti biti.
	*****************************************************************/
	provere.resize(matrica[0].size());

	for (unsigned int x = 0; x < matrica[0].size(); ++x)
	{
		for (unsigned int y = 0; y < matrica.size(); ++y)
		{
			if (matrica[y][x] == 1) provere[x].push_back(y);
		}
	}
}

void nadjiBite(vector<vector<int>>& matrica, vector<vector<int>>& biti)
{
	/***************************************************************
	Funkcija pravi skup od N bita koji ucestvuju u M-tim proverama.
	***************************************************************/
	biti.resize(matrica.size());
	for (unsigned int y = 0; y < matrica.size(); ++y)
	{
		for (unsigned int x = 0; x < matrica[0].size(); ++x)
		{
			if (matrica[y][x] == 1) biti[y].push_back(x);
		}
	}
}

void inicExtrinsic(vector<vector<double>>& extrinsic, vector<vector<int>>& provere)
{
	/***************************************************************
	Funkcija inicijalizuje extrinsic vrednosti (postavlja ih na 0).
	***************************************************************/
	extrinsic.resize(provere.size());
	for (unsigned int i = 0; i < provere.size(); ++i)
	{
		extrinsic[i].resize(provere[i].size());
		for (unsigned int j = 0; j < provere[i].size(); ++j) extrinsic[i][j] = 0;
	}
}

void XuC(vector<double>& aposteriori, vector<vector<double>>& extrinsic, vector<vector<int>>& provere, vector<vector<double>>& C)
{
	/************************************************************************
	Funkcija salje primljenu sekvencu (lambda) u dekoder (C).
	Pri prvom salju prima se samo lambda vrednost,
	a u ostalim slanjima od aposteriori oduzimamo trazenu extrinsic vrednost
	*************************************************************************/
	for (unsigned int i = 0; i < C.size(); ++i) C[i].clear();  /// inicijalizuj C

	for (unsigned int i = 0; i < aposteriori.size(); ++i)
	{
		for (unsigned int j = 0; j < provere[i].size(); ++j)
		{
			C[provere[i][j]].push_back(aposteriori[i] - extrinsic[i][j]);
		}
	}
}




void proveraParnosti(vector<vector<double>>& C, vector<int>& greske, int iteracija)
{
	int pozitivni, negativni;
	for (unsigned int provera = 0; provera < C.size(); ++provera)
	{
		pozitivni = 0;
		negativni = 0;
		for (unsigned int bit = 0; bit < C[provera].size(); ++bit)
		{
			if (C[provera][bit] >= 0) ++pozitivni;
			else ++negativni;
		}
		if (pozitivni != negativni)
		{
			++greske[iteracija];
			break;
		}
	}
}




void MinSum(vector<vector<double>>& extrinsic, vector<vector<int>>& biti, vector<vector<double>>& C)
{
	/*****************************************************
	Funkcija trazi min sum vrednosti pristiglih u dekoder
	*****************************************************/
	for (unsigned int x = 0; x < extrinsic.size(); ++x) extrinsic[x].clear();  /// inicijalizuj extrinsic

	for (unsigned int i = 0; i < C.size(); ++i)
	{
		for (unsigned int j = 0; j < C[i].size(); ++j)
		{
			double tmp;
			bool znak(false);

			/// Inicijalizacija tmp promenljive
			if (j == 0) tmp = abs(C[i][1]);
			else tmp = abs(C[i][0]);

			for (unsigned int k = 0; k < C[i].size(); ++k)
			{
				if ((k != j) && (C[i][k] < 0)) znak = !znak;
				if ((k != j) && (abs(C[i][k]) < tmp)) tmp = abs(C[i][k]);
			}

			if (znak == true) tmp = -tmp;
			extrinsic[biti[i][j]].push_back(tmp);
		}
	}
}

void getAposteriori(vector<double>& lambda, vector<double>& aposteriori, vector<vector<double>>& extrinsic)
{
	/*************************************************
	Funkcija racuna aposteriori vrednosti na osnovu
	lambda vrednosti i proracunatih min sum vrednosti
	*************************************************/
	for (unsigned int i = 0; i < lambda.size(); ++i)
	{
		double suma(0);

		for (unsigned int j = 0; j < extrinsic[i].size(); ++j)
		{
			suma += extrinsic[i][j];
		}
		aposteriori[i] = lambda[i] + suma;
	}
}


void provera(vector<double>& kodnaRec, vector<double>& aposteriori, vector<vector<int>>& fer, vector<vector<double>>& ber, vector<int>& greske, int brojSekvence)
{
	double brojGresaka(0);
	bool identicni(true);

	for (unsigned int i = 0; i < kodnaRec.size(); ++i)
	{
		if (((kodnaRec[i] == 1.0) && (aposteriori[i] < 0.0)) || ((kodnaRec[i] == -1.0) && (aposteriori[i] >= 0.0)))
		{
			identicni = false;
			++brojGresaka;
		}
	}

	if (identicni == true) fer[brojSekvence].push_back(0);
	else
	{
		fer[brojSekvence].push_back(1);
		++greske[brojSekvence];
	}

	ber[brojSekvence].push_back((double)(brojGresaka / kodnaRec.size()));
}


/*
void provera(vector<double>& kodnaRec, vector<double>& aposteriori, vector<vector<int>>& fer, vector<vector<double>>& ber, int brojSekvence)
{
	double brojGresaka(0);
	bool identicni(true);

	for (unsigned int i = 0; i < kodnaRec.size(); ++i)
	{
		if (((kodnaRec[i] == 1.0) && (aposteriori[i] < 0.0)) || ((kodnaRec[i] == -1.0) && (aposteriori[i] >= 0.0)))
		{
			identicni = false;
			++brojGresaka;
		}
	}

	if (identicni == true) fer[brojSekvence].push_back(0);
	else
	{
		fer[brojSekvence].push_back(1);
	}

	ber[brojSekvence].push_back((double)(brojGresaka / kodnaRec.size()));
}
*/
