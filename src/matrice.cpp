#include "matrice.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <iso646.h>

using namespace std;


/***** FUNKCIJE ZA PRAVLJENJE MATRICE *****/

void inicTraku(vector<vector<bool>>& traka, const int& j, const int& k, const int& M)
{
	/*********************************************************************
	Funkcija za inicijalizaciju trake za pravljenje Gallager-ove matrice.
	Pravi traku oblika:
	111000000
	000111000
	000000111    sa M/j vrsta i k*M/j kolona

	traka - vektor u koji se traka smesta
	j - broj jedinica po koloni
	k - broj jedinica po vrsti
	*********************************************************************/
	for(int y = 0; y < M / j; ++y)
	{
		for(int x = y * k; x < y * k + k; ++x)
		{
			traka[y][x] = 1;
		}
	}
}

void shuffle(vector<vector<bool>>& traka)
{
	/***************************************************
	Funkcija nasumicno menja kolone u matrici, dva puta
	***************************************************/
	int tmp, br(2);

	while(br != 0)
	{
		for(unsigned int i = 0; i < traka[0].size(); ++i)
		{
			unsigned int kolona, seed;
			LARGE_INTEGER li;
			/******************************************
			kolona - nasumicno odabrana kolona matrice
			seed - seed za RNG
			li - veliki int za seed
			******************************************/

			QueryPerformanceCounter(&li);
			seed = li.LowPart;
			mt19937 eng(seed); // engine: Mersenne Twister 19937 generator
			uniform_int_distribution<int> dist(0, traka[0].size() - 1); // uniformna distribucija
			kolona = dist(eng);

			for(unsigned int j = 0; j < traka.size(); ++j)
			{
				tmp = traka[j][i];
				traka[j][i] = traka[j][kolona];
				traka[j][kolona] = tmp;
			}
		}
		--br;
	}
}

void makeMatrix(vector<vector<bool>>& H, vector<vector<bool>>& traka, int j)
{
	/*******************************************
	Funkcija popunjava matricu, traku po traku.

	H - vektor u koji se smesta matrica
	traka - traka kojom se popunjava matrica
	j - broj jedinica po koloni
	*******************************************/
	for(int i = 0; i < j; ++i)
	{
		for(unsigned int y = 0; y < traka.size(); ++y)
		{
			for(unsigned int x = 0; x < traka[0].size(); ++x)
			{
				H[i * traka.size() + y][x] = traka[y][x];
			}
		}
		shuffle(traka);
	}
}



/***** I/O *****/

void txt2matrix(string s, vector<vector<int>>& mat)
{
	fstream f;
	int red(0);
	char c;

	f.open(s, fstream::in);

	if (f.is_open())
	{
		while (!f.eof())
		{
			c = f.get();
			switch (c)
			{
			case '0':
				mat[red].push_back(0);
				break;
			case '1':
				mat[red].push_back(1);
				break;
			case '\n':
				if (f.peek() != EOF)
				{
					mat.push_back(vector<int>(0));
					++red;
				}
			}
		}
		f.close();
	}
	else cout << "\n\n!!!GRESKA PRI OTVARANJU FAJLA!!!\n\n";
}


void mat2txt(ofstream& f, vector<vector<bool>>& mat)
{
	/************************************
	Funkcija smesta matricu u .txt fajl.
	************************************/
	for(unsigned int y = 0; y < mat.size(); ++y)
	{
		for(unsigned int x = 0; x < mat[0].size(); ++x)
		{
			f << mat[y][x];
		}
		f << endl;
	}
}


/***** FUNKCIJE ZA GAUSOVU ELIMINACIJU *****/
bool findOneAbove(vector<vector<bool>>& mat, int x, int y)
{
	/*********************************************
	Funkcija trazi jedinice iznad zeljenog mesta.
	Ako nadje jednu, zameni mesta zeljenom redu i
	redu sa jedinicom, a zatim vraca true. Ako ne
	nadje ni jednu jedinicu, vraca se false.
	*********************************************/
	bool tmp, res(false);

	for(int i = y; i >= 0; --i)
	{
		if(mat[i][x] == 1)
		{
			for(unsigned int xprim = 0; xprim < mat[0].size(); ++xprim)
			{
				tmp = mat[y][xprim];
				mat[y][xprim] = mat[i][xprim];
				mat[i][xprim] = tmp;
			}
			res = true;
			break;
		}
	}
	return res;
}

bool findOneLeft(vector<vector<bool>>& mat, vector<vector<bool>>& mpp, int x, int y)
{
    /************************************************************
    Funkcija trazi jedinice levo od zeljenog mesta u istom redu.
    Vraca true ako je pronasla, a kolone zamenjuju mesta,
    i u originalnoj i u matrici provere parnosti.
    Ako se ne pronadje ni jedna jedinica, vraca se false.
    ************************************************************/
    bool tmp, res(false);

    for(int i = x; i >= 0; --i)
	{
		if(mpp[y][i] == 1)
		{
			/***** zamena kolona u matrici provere parnosti *****/
			for(unsigned int yprim = 0; yprim < mpp.size(); ++yprim)
			{
				tmp = mpp[yprim][x];
				mpp[yprim][x] = mpp[yprim][i];
				mpp[yprim][i] = tmp;
			}

			/***** zamena kolona u originalnoj matrici *****/
			for(unsigned int yprim = 0; yprim < mat.size(); ++yprim)
			{
				tmp = mat[yprim][x];
				mat[yprim][x] = mat[yprim][i];
				mat[yprim][i] = tmp;
			}
			res = true;
			break;
		}
	}
	return res;
}

bool findOneInRect(vector<vector<bool>>& mat, vector<vector<bool>>& mpp, int x, int y)
{
	/**********************************************************************
	Funkcija trazi jedinicu u pravougaoniku iznad i levo od zeljenog mesta
	**********************************************************************/
	bool res(false);

	for(int i = x - 1; i >= 0; --i)
	{
		if(findOneAbove(mpp, i, y))
		{
            if(findOneLeft(mat, mpp, x, y))
			{
				res = true;
			}
            break;
		}
	}
	return res;
}

void removeOtherOnes(vector<vector<bool>>& mat, int x, int y)
{
	/********************************************************************************
	Funkcija eliminise jedinice iznad i ispod zeljenog mesta koristeci xor operaciju
	********************************************************************************/
	for(unsigned int i = 0; i < mat.size(); ++i)
	{
		if((mat[i][x] == 1) && (i != y))
		{
			/** nasli smo jedinicu, eliminisemo je xor operacijom **/
			for(unsigned int xprim = 0; xprim < mat[0].size(); ++xprim)
			{
				mat[i][xprim] = mat[i][xprim] xor mat[y][xprim];
			}
		}
	}
}

void gaussElimination(vector<vector<bool>>& H, vector<vector<bool>>& gen)
{
	/*********************************************************************
	Funkcija pravi matricu provere parnosti koristeci Gausovu eliminaciju
	*********************************************************************/
	int x;
	bool kraj(false);
	vector<vector<bool>> mpp;

	mpp = H;
	x = mpp[0].size();

	for(int y = mpp.size() - 1; y >= 0; --y)
	{
		--x;
		/** provera da li je na zeljenom mestu nula **/
        if(mpp[y][x] == 0)
		{
			/** provera da li ima jedinica iznad zeljenog mesta **/
            if(!findOneAbove(mpp, x, y))
			{
				/** provera da li ima jedinica levo od zeljenog mesta **/
				if(!findOneLeft(H, mpp, x, y))
				{
					/** provera da li ima jedinica levo i iznad zeljenog mesta **/
					if(!findOneInRect(H, mpp, x, y))
					{
						/** nema vise jedinica, ostali redovi su popunjeni nulama **/
                        while(y >= 0)
						{
							mpp.erase(mpp.begin());
							--y;
						}
						kraj = true;
						break;
					}
				}
			}
		}
		if(!kraj) removeOtherOnes(mpp, x, y);
	}

	/** pronadjena matrica provere parnosti, kreiraj generatorsku matricu **/
	int identStart; // pozicija (kolona) na kojoj pocinje identity matrica
	identStart = mpp[0].size() - mpp.size();


	/** pravimo I matricu od gen matrice **/
	gen.resize(identStart);
	for (int i = 0; i < identStart; ++i)
	{
        gen[i].resize(identStart);
        gen[i][i] = 1;
	}

	/** dodaj na kraj gen matrice transponovanu mpp **/
	for(unsigned int genRed = 0; genRed < gen.size(); ++genRed)
	{
		for(unsigned int mppRed = 0; mppRed < mpp.size(); ++mppRed)
		{
            gen[genRed].push_back(mpp[mppRed][genRed]);
		}
	}
}
