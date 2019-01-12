#include "randnum.h"
#include "matrice.h"
#include "koder.h"
#include "dekoder.h"
#include "statistika.h"
#include "pomocne.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	double SNR(-2.0);  // Odnos signal - sum

	vector<vector<int>> H_matrica(1, vector<int>(0));  // Matrica provere parnosti
	vector<vector<int>> generatorska_matrica(1, vector<int>(0));  // Generatorska matrica

	vector<int> infSekvenca;
	vector<double> kodnaRec;
	vector<double> lambda;
	vector<double> aposteriori;

	vector<vector<int>> fer;
	vector<vector<double>> ber;
	vector<double> RoFER(100);
	vector<double> RoBER(100);
	vector<int> greske(100);
	vector<double> StatFER(100);
	vector<double> StatBER(100);

	vector<vector<int>> dekoderskeProvere;
	vector<vector<int>> dekoderskiBiti;
	vector<vector<double>> extrinsic;

	fstream f;

	double eps;  // epsilon - najmanja masinska vrednost
	LARGE_INTEGER li;
	RandomData rd;  // kontejner sa podacima za generator nasumicnih brojeva

	bool ok, traziRo;
	int brojSekvence;

	/** Odredjivanje pocetnog stanja kola za kasnjenje **/
	QueryPerformanceCounter(&li);
	rd.d1 = 10000000.0 / li.LowPart;
	QueryPerformanceCounter(&li);
	rd.d2 = 100000000.0 / li.LowPart;
	rd.a1 = 1;
	rd.a2 = 1;

	/** Odredjivanje vrednosti epsilon **/
	eps = numeric_limits<double>::epsilon();
	rd.c = (2 * eps) / 3;

	/** Inicijalizacija niza nasumicnih brojeva **/
	for (unsigned int i = 0; i < 128; ++i)
	{
		rd.ra[i] = rng(rd);
	}

	/** Ucitavanje matrice H i generatorske matrice **/
	txt2matrix("matrix.txt", H_matrica);
	txt2matrix("generator.txt", generatorska_matrica);

	/** Inicijalizacija dekodera **/
	nadjiProvere(H_matrica, dekoderskeProvere);
	nadjiBite(H_matrica, dekoderskiBiti);
	vector<vector<double>> C(dekoderskiBiti.size(), vector<double>(0));

	/** Brisanje postojecih FER i BER fajlova **/
	f.open("BER.txt", fstream::out);
	f.close();
	f.open("FER.txt", fstream::out);
	f.close();


	/** SPOLJASNJA PETLJA - SNR **/
	while (SNR < 3.1) // promeni posle u while(1)
	{
		if ((SNR > -0.01) && (SNR < 0.1)) SNR = 0;
		fer.clear();
		ber.clear();
		vector<int> greske(100, 0); // Reinicijalizacija vektora
		ok = false;
		brojSekvence = 0;


		cout << endl << SNR << ": "; /// TMP

		/** SREDNJA PETLJA - PO KODNIM RECIMA **/
		do
		{
			fer.push_back(vector<int>(0));
			ber.push_back(vector<double>(0));

			/** Informaciona sekvenca i kanal **/
			makeInfSekvence(infSekvenca, generatorska_matrica, rd);



			//cout << endl << proveriMatrice(H_matrica, generatorska_matrica) << endl;
			//napraviKodnuRec(kodnaRec, infSekvenca, generatorska_matrica); ///TMP
			//cout << endl << proveriKodnuRec(kodnaRec, H_matrica) << endl; ///TMP





			coder(kodnaRec, infSekvenca, generatorska_matrica);
			channelTransfer(kodnaRec, lambda, SNR, rd);
			//lambda = kodnaRec; /// TMP

			/** Inicijalizacija za prvo slanje **/
			aposteriori.resize(lambda.size());
			aposteriori = lambda;
			inicExtrinsic(extrinsic, dekoderskeProvere);

			cout << "*"; /// TMP

			/** PETLJA PO ITERACIJAMA (DEKODER) - 100 ITERACIJA **/
			for (int i = 0; i < 100; ++i)
			{
				XuC(aposteriori, extrinsic, dekoderskeProvere, C);
				//proveraParnosti(C, greske, i);
				MinSum(extrinsic, dekoderskiBiti, C);
				getAposteriori(lambda, aposteriori, extrinsic);
				//provera(kodnaRec, aposteriori, fer, ber, brojSekvence);
				provera(kodnaRec, aposteriori, fer, ber, greske, brojSekvence);


				//stanje(kodnaRec, aposteriori);
			}

			traziRo = false;
			/** Da li je u svakoj iteraciji bilo bar po 4 greske? **/
			if (brojSekvence > 4)  // ne proveravamo dok ne prodju bar 4 sekvence, a time i dok se ne skupe bar 4 greske po iteraciji
			{
				for (int iter = 0; iter < 100; ++iter)
				{
					if (greske[iter] < 5)
					{
						traziRo = false;
						break;
					}
					else traziRo = true;
				}
			}

			if (traziRo == true)
			{
				findRhoFER(fer, StatFER, RoFER);
				findRhoBER(ber, StatBER, RoBER);

				ok = true;
				for (unsigned int i = 0; i < RoFER.size(); ++i)
				{
					if ((RoFER[i] > 0.2) || (RoBER[i] > 0.2))
					{
						ok = false;
						break;
					}
				}
			}

			++brojSekvence;

		}while (ok == false);

		writeFER(StatFER, SNR);
		writeBER(StatBER, SNR);

		SNR += 0.1;
	}


	cout << "\nPress ENTER to end.\n";
	cin.ignore();
	return 0;
}
