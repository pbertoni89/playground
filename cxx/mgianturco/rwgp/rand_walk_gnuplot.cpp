/**
 * Random Walk + Grafica Gnuplot
 * 10 gennaio 2019
 * Autori:
 *  - pbertoni
 *  - mgianturco
 * 
 * 
 */

/*
	COMPORTAMENTO R.W
 Posizione media dopo n passi

 x(medio, posizione del random walkers dopo n passi) = n(1) + n(-1) = N(P(verso destra) - P(passo verso sinistra)) = 0
*/

#include <iostream>
#include <fstream>
#include <cstdlib>


using namespace std;


/**
 * Se posso, unisco dichiarazione + definizione di una funzione in un solo punto.
 * In questo modo vedo sempre subito le mie funzioni.
 */
void attendi (float s)
{
	clock_t t=clock();

	// se.ioTi-scrivessi(in_italiano)cosìtuciCapiresti?   USIAMO GLI SPAZI
	// while (clock()-t<s*CLOCKS_PER_SEC)
	while (clock() - t < s * CLOCKS_PER_SEC)
	{}
}



/**
 * Seconda parte dell'esercizio: i camminatori casuali che verranno graficati (plottati) con gnuplot.
 * Siccome il main() sta diventando lunghissimo, e inizio a non capirci più niente,
 * Creo un altra funzione per dividere il codice in parti più piccole e leggibili.
 */
void walker_gnuplot_ranger()
{
	/**
	 * Dichiara un ofstream (Output File Stream) e lo chiama 'fout'.
	 * A parte rassomigliare a un insulto in inglese molto abbreviato,
	 * cosa TI DICE 'fout' ? un bel NIENTE sul suo contenuto.
	 *
	 * Vado avanti di una riga, ah, INTUISCO a fatica che è il file dove salvo
	 * le posizioni dei miei N camminatori casuali.
	 *
	 * Quindi rinomino la variabile 'fout' in 'fout_posizioni', e il file su cui
	 * essa scriverà, 'pos.txt', lo rinomino 'posizioni.txt'.
	 *
	 * I caratteri sono gratuiti, non siamo più negli anni Ottanta.
	 */
	// ofstream fout;
	ofstream fout_posizioni;
	fout_posizioni.open("posizioni.txt");
	
	// li avevo già dichiarati... ma nel main. Qua siamo in un'altra funzione, per il compilatore non esistono!
	int T, t, e;
	int N;
	cout << "Quanti random walkers" << endl;
	cin >> N;
	
	cout << "Periodo: " << endl;
	cin >> T;
	
	int y[N]; //l'array contiene la posizione "attuale" di ciascun random walker
	for (int i = 0; i < N; i++)
		y[i] = 0;

	float media = 0;
	t = 0;
	while(t < T)
	{
		fout_posizioni << t << " ";
		for(int i = 0; i < N; i ++)
		{
			e = rand()%2;
			if (e == 1)
			{
				y[i] = y[i] + 1;
				fout_posizioni << y[i] << " ";
				
			}
			else
			{
				y[i] = y[i] - 1;
				fout_posizioni << y[i] << " ";
			}
			// media += media + y[i]; // sommo gli N valori della posizione degli N r.w
			media = media + y[i]; // errore, sommavo due volte!! Attenzione a +=
		}
		fout_posizioni << " " << media / N << endl; //faccio la media delle posizioni
		// fout << endl;
		media = 0;
		t++;
	}
	// è sempre buona cosa chiudere un file per dire: ho scritto tutto quello che dovevo scrivere
	fout_posizioni.close();

	/* stesso ragionamento: 'fout2' mi dice ancora meno di 'fout'.. mi dice che è il secondo
	 * scrittore su file ma non mi dice quale file nè che senso hanno i caratteri che scriverà
	 * abbiamo però visto che il programma Gnuplot necessita di DUE cose
	 *  - posizioni (COSA plottare)
	 *  - istruzioni (COME farlo)
	 * che cosa significa 'graph.txt' ? NIENTE
	 */
	// ofstream fout2;
	ofstream fout_istruzioni;
	fout_istruzioni.open("istruzioni.txt");

	fout_istruzioni << "unset key" << endl; //togliere la legenda
	fout_istruzioni << "plot";

	// PER OGNI walker, istruisco gnuplot su quali sono i SUOI dati da plottare
	for (int i = 2; i <= N + 2; i++)
	{
		fout_istruzioni << "'posizioni.txt' u 1:" << i;
		if (i <= N + 1)
		{
			fout_istruzioni << "w l, ";
		}
		else
		{
			fout_istruzioni << " w p";
		}
	}
	
	fout_istruzioni << endl;
	fout_istruzioni.close();
	system ("gnuplot -p istruzioni.txt");
}



/**
 * Cosa fa questa regola? è il testa-croce ideale: 50% casi vado a sx, 50% a dx
 */
int rule5050 (int x)
{
	int e;
	e=rand()%2;
	if (e==1)
		x=x+1;
	else
		x=x-1;
	return x;
}



/**
 * Cosa fa questa regola? 25% vado a sx, 25% a dx, 50% non mi muovo
 */
int rule252550(int x)
{
	int e;
	e=rand()%4; //genera numeri casuali tra 0 e 3.
	if (e==0)
		x=x-1;
	else if (e==1)
		x=x+1;
	else
		x=x;
	return x;
}



int main ()
{
	srand(time(NULL));

	ifstream input;
	system ("tput cols > cols.txt");
	input.open("cols.txt");
	int COLS;
	input >> COLS;
	input.close ();

	int x = COLS/2;

	/**
	 * T, s... cosa vogliono dire? Cerchiamo di dargli un senso
	 */
	int T, s;

	// ah ecco il senso di 's': è la regola. Perchè non rinominarla in 'regola' ?
	cout << "Quale regola vuoi per il r.w.? [50] 50 oppure [25] 25 50?" << endl;
	cin >> s;

	cout <<"Quanto tempo durerà la simulazione del moto del r.w.?"<< endl;
	cin >> T;

	int t=0;
	int e;
	system("clear");

	while (t<T)
	{
		// l'argomento è la posizione precedente e viene restituita la nuova posizione
		if (s==50)
			x=rule5050(x);
		else
			x=rule252550(x);

		//system("clear");
		for (int i=0; i<(COLS-1); i++)
		{
			cout << "▒"; 
			if (i==x)
				cout << "█";
		}
		cout << endl;
		attendi(0.1);
		t++;
	}
	cout << endl;


	char scelta;
	cout << "Vuoi proseguire con più random walkers?" << endl;
	cin >> scelta;

	if (scelta == 'y' or scelta == 'Y')
	{
		walker_gnuplot_ranger();
	}

	return 0;
}
