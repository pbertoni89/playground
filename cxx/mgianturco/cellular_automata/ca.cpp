#include <iostream>

// mi serve per chiamare la funzione exit()
#include <cstdlib>

using namespace std;

#define BIANCO "░░"
#define NERO "▓▓"


// Fissiamo la lunghezza della sequenza e il suo periodo di evoluzione
#define AC_SEQUENZA_LUNGHEZZA 41
// n è un SINONIMO di AC_SEQUENZA_LUNGHEZZA
#define n AC_SEQUENZA_LUNGHEZZA

#define AC_SEQUENZA_PERIODO 30
#define T AC_SEQUENZA_PERIODO

/*
 * Ora, stando qua - fuori da ogni funzione - sono due array di int dichiarati
 * come variabili GLOBALI
 */
int seq[n], seq_temp[n];

// Il vettore dov'è definita la regola. esempio: 30 -> 11110
#define COMBINAZIONI 8
bool vett_regola[COMBINAZIONI] = {0,0,0,0,0,0,0,0};


/**
 * 
 * @param a ?
 * @param r ?
 * @return ?
 */
int evolvi_cella (int a, int r)
{
	int i = 0;

	while (r != 0)
	{
		if (i < 0 or i >= COMBINAZIONI)
		{
			cout << "ACCESSO IMPOSSIBILE: i = " << i;
			exit(2);
		}
		vett_regola[i] = r % 2;
		i++;
		r = r / 2;
	}

	if (a < 0 or a >= 8)
	{
		cout << "ACCESSO IMPOSSIBILE: a = " << a;
		exit(1);
	}
	// in base al vicinato la cella iesima evolve come la cifra di v[]
	return (vett_regola[a]);
}


/**
 * Data una certa regola, la svolge tutta (per tutto il periodo di evoluzione)
 * e la stampa. All'inizio per comodità su cout, poi la scriveremo su file
 *
 * @param r - il numero decimale della regola
 * @return nulla di importante. potrebbe essere void
 */
void scrivi_regola (int r)
{
	// Genero e disegno la condizione iniziale
	for (int i = 0; i < n; i++)
	{
		if (i == (n/2))
		{
			seq[i] = 1;
			cout << NERO;
		}
		else
		{
			seq[i] = 0;
			cout << BIANCO;
		}
	}
	cout << endl;

	// Parto da 1 poichè considero t==0 come la condizione iniziale
	int t = 1;

	//identifica lo stato del vicinato: è un numero intero che va da 0 a 7
	int a = 0;

	// Disegno l'evoluzione dell'Automa Cellulare
	while (t < T)
	{
		// copia dello stato attuale in seq_temp[]
		// Devo farla, altrimenti pasticcerei di certo l'evoluzione
		for (int i = 0; i < n; i++)
		{
			seq_temp[i] = seq[i];
		}

		// Evoluzione della sequenza
		for (int i = 0; i < n; i++)
		{
			// scrivo in "a" in binario il valore della sequenza
			a = seq_temp[i] * 2;
			if (i > 0)
			{
				a += seq_temp[i-1] * 4;
			}
			if (i < n - 1)
			{
				a += seq_temp[i+1] * 1;
			}
			seq[i] = evolvi_cella(a, r);
		}

		// Disegna la sequenza
		for (int i = 0; i < n; i++)
		{
			if (seq[i] == 0)
				cout << BIANCO;
			else
				cout << NERO;
		}
		cout << endl;
		t++;
	}
}


/**
 * Il main ci DEVE sempre essere !!!
 * @return 
 */
int main()
{
	cout << "Automi Cellulari - Lunghezza " << n << ", Tempo " << T << endl;
	for (int i=0; i<256; i++)
	{
		cout << "Regola ° " << i << endl;
		scrivi_regola(i);
	}
	cout << "Fine programma" << endl;
}
