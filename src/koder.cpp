#include "koder.h"
#include <cmath>
#include <iso646.h>

using namespace std;

/************************************ Funkcije za koder ************************************/
void makeInfSekvence(vector<int>& infSekvenca, vector<vector<int>>& gen, RandomData& rd)
{
	/**********************************************
	Funkcija nasumicno pravi informacionu sekvencu
	**********************************************/
	infSekvenca.clear();
	for(unsigned int i = 0; i < gen.size(); ++i)
	{
		infSekvenca.push_back(bin(get_rn(rd)));
	}
}

void coder(vector<double>& kodnaRec, vector<int>& infSekvenca, vector<vector<int>>& gen)
{
	/***********************************************************************
	Funkcija kodera pravi kodnu rec tako sto informacionu sekvencu mnozi sa
	generatorskom matricom. Nule postaju 1, a jedinice -1.
	***********************************************************************/
	kodnaRec.clear();
	for(unsigned int i = 0; i < gen[0].size(); ++i)
	{
		bool tmp(false);
		for(unsigned int j = 0; j < gen.size(); ++j)
		{
			tmp = tmp xor (bool) (infSekvenca[j] * gen[j][i]);
		}
		if(tmp == false) kodnaRec.push_back(1.0);
		else kodnaRec.push_back(-1.0);
	}
}


/************************************ Funkcije za kanal ************************************/
void channelTransfer(vector<double>& kodnaRec, vector<double>& lambda, double SNR, RandomData& rd)
{
	/********************************************************************
	Funkcija simulira kanal tako sto dodaje sum na informacionu sekvencu
	i pravi lambda vrednosti od informacione sekvence.
	********************************************************************/
	double sigma;

	lambda.clear();
	sigma = pow(10.0, (- SNR / 20.0));

	for(unsigned int i = 0; i < kodnaRec.size(); ++i)
	{
		lambda.push_back(2 * (kodnaRec[i] + gaus(rd) * sigma) / (sigma * sigma));
	}
}
