#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cmath>

using namespace std;

#define DIM 32767

/**
 * se il carattere è uno dei caratteri ascii corrispondenti a un numero
 * @param a
 * @return 
 */
bool is_number(char a)
{
	if (a>=48 and a<=57)
		return true;
	else
		return false;
}


bool is_operator(char a)
{
	if (a == '+' or a == '-' or a == '*' or a == '/')
		return true;
	else
		return false;
}


bool is_add_diff(char a)
{
	if (a == '+' or a == '-')
		return true;
	else
		return false;
}


bool is_mul_div(char a)
{
	return is_operator(a) and (is_add_diff(a) == false);
}


int operazione(int numSx, int numDx, char op)
{
	switch (op)
	{
		case '+':
			return numSx + numDx;
		case '-':
			// sommo comunque; ho già considerato i negativi
			return numSx + numDx;
		case '*':
			return numSx * numDx;
		case '/':
			return numSx / numDx;
		case '\0':
			return numSx;
	}
}


/**
 * Non studiarla particolarmente. Serve per aiutarci nel test di correttezza
 * @param espressione
 * @return il risultato esatto dell'espressione
 */
int risultato_esatto(char * espressione)
{
	char comando[1024];
	sprintf(comando, "echo $((%s))", espressione);
	char buf[1024];
	FILE * fp;
	int risultato;

	if ((fp = popen(comando, "r")) != NULL)
	{
		while (fgets(buf, 1024, fp) != NULL)
		{
			risultato = atoi(buf);
		}
		pclose(fp);
	}
	return risultato;
}

// commenta questa riga per caricare un espressione e debuggare rapidamente
// #define CHIEDI_DA_TASTIERA

/**
 * Limiti del programma attuali: si segnala che con numeri grandi si ottengono
 * risultati errati. Quanto grandi ? esempio:
 *		4+54221321312144*3*2-4/2+1*4-8
 * Mentre con meno grandi, funziona. esempio
 *		4+54244*3*2-4/2+1*4-8
 * Questo perchè il tipo INT ha dei limiti e va in OVERFLOW ARITMETICO.
 * Se si vogliono gestire numeri più grandi, occorre passare ai LONG
 */
int main()
{
	char expression[DIM];
#ifdef CHIEDI_DA_TASTIERA
	cout << "Scrivi l'espressione da risolvere: " << endl;
	cin.getline(expression, DIM);
#else
	snprintf(expression, DIM, "4+54244*3*2-4/2+1*4-8");
	cout << "precarico l'espressione " << expression << endl;
#endif
	
	// numeri = o addendi +,- o prodotti *,/
	int numeri[DIM];
	int addendi[DIM];
	int prodotti[DIM];
	for (int i=0; i<DIM; i++)
	{
		numeri[i] = 0;
		addendi[i] = 0;
		prodotti[i] = 0;
	}
	char operatori[DIM];
	int idx_num = 0, idx_op = 0, idx_prod = 0, idx_add = 0;

	// Primo Ciclo: ricerca dei numeri e degli operatori
	for (int i=0; i<=strlen(expression); i++)
	{
		char C = expression[i];
		if (is_operator(C) or C=='\0')
		{
			// serve per contare le potenze di 10
			int conta = 0;

			// il ciclo dura fintanto che il carattere è un numero
			int j;
			for (j = i-1; is_number(expression[j]); j--)
			{
				numeri[idx_num] = numeri[idx_num] + (expression[j]-48) * pow(10, conta);
				conta++; //aumenta l'esponente della potenza di 10
			}

			// è un numero negativo ?
			if (j > 0 and expression[j] == '-')
			{
				// ribalto il numero
				numeri[idx_num] = - numeri[idx_num];
				// a sto punto è una somma con un negativo
				// operatori[idx_op-1] = '+';
			}

			cout << "num[" << idx_num << "]: " << numeri[idx_num] << endl;
			idx_num++;

			// il terminatore NON è un operatore
			//if (C != '\0')
			{
				operatori[idx_op] = C;
				cout << "ope[" << idx_op << "]: " << operatori[idx_op] << endl;
				idx_op++;
			}
		}
	}

	// spazio
	cout << endl;

	// Secondo Ciclo: consumo tutte le moltiplicazioni e divisioni
	int iop_i, iop_j;
	int N_OPS = strlen(operatori);
	for (iop_i=0; iop_i<=N_OPS; iop_i++)
	{
		// i due numeri sinistro e destro dell'operazione corrente
		int numSx = numeri[iop_i], numDx = numeri[iop_i+1];
		// l'operazione corrente
		char op_i = operatori[iop_i], op_j;
		cout << "controllo ope[" << iop_i << "] = " << op_i << " tra " << numSx << " e " << numDx << endl;

		if (is_mul_div(op_i))
		{
			// inizializzo il prodotto cumulato esempio di 3*4/6*7 (3)
			int prodotto_cumulato = numSx;
			bool continua_prod = false;
			iop_j = iop_i;
			// finchè ci sono moltiplicazioni/divisioni consecutive
			do
			{
				op_j = operatori[iop_j];
				cout << "\tprodotto_cumulato = operazione(" << prodotto_cumulato << ", " << numeri[iop_j+1] << ", " << op_j << ") = ";
				prodotto_cumulato = operazione(prodotto_cumulato, numeri[iop_j+1], op_j);
				cout << prodotto_cumulato << endl;
				
				// mi sposto alla ricerca di una *,/ contigua alla precedente
				iop_j ++;
				// continuo se la prossima operazione è ancora un prodotto
				op_j = operatori[iop_j];
				continua_prod = is_mul_div(op_j);
				cout << "\tcontinuo se ope[" << iop_j << "] = " << op_j << " è *o/: " << (continua_prod ? "sì" : "no") << endl;
			}
			while (continua_prod and iop_j < N_OPS);
			
			cout << "\tsetto i <- j cioè " << iop_i << " <- " << iop_j << endl;
			iop_i = iop_j;

			// ho finito una sequenza di prodotti: la salvo
			prodotti[idx_prod] = prodotto_cumulato;
			cout << "\tprod[" << idx_prod << "]: " << prodotto_cumulato << endl;
			idx_prod++;
		}
		else
		{
			addendi[idx_add] = numSx;
			cout << "\tadd[" << idx_add << "]: " << numSx << endl;
			idx_add++;
		}
	}

	// spazio
	cout << endl;

	// Terzo Ciclo: ora posso sommare gli addendi
	int risultato = 0;
	for (int i=0; i<idx_add; i++)
	{
		// qua nemmeno uso più l array operatori: devo solo sommare delle quantità positive o negative
		cout << "somma con addendi: " << risultato << " + " << addendi[i];
		risultato = risultato + addendi[i];
		cout << " = " << risultato << endl;
	}
	// Quarto Ciclo: ora posso sommare i prodotti
	for (int i=0; i<idx_prod; i++)
	{
		// qua nemmeno uso più l array operatori: devo solo sommare delle quantità positive o negative
		cout << "somma con prodotti: " << risultato << " + " << prodotti[i];
		risultato = risultato + prodotti[i];
		cout << " = " << risultato << endl;
	}
	
	int risultato_atteso = risultato_esatto(expression);
	cout << endl << "risultato ottenuto: " << risultato << ", atteso: " << risultato_atteso << endl;

	bool retval = (risultato != risultato_atteso);

	return retval;
}
