/********************************************************************************
*                                                                              *
*                   Architetture dei sistemi di Elaborazione                   *
*                                                                              *
********************************************************************************

Elaborato 2
Descrizione: Dato in input un numero naturale n, restituire i primi n termini
			della successione di Fibonacci.

********************************************************************************/

#include <stdio.h>

void main()
{
	//Variabili
	int n = 1;						//Numero di termini da restituire
	int successione[50];			//Vettore in cui mettere i primi n termini della successione di Fibonacci

	//Blocco Assembler
	__asm
	{
		xor eax, eax; azzeramento reg eax
		mov ebx, 1
		mov ecx, n; counter fino nth elemento di fibonacci
		xor edx, edx; azzeramento reg edx

		mov successione[ECX * 4], 0; 1st elemento di fib è 0
		cmp ecx, 0; se ho inserito n = 0
		je Fine; devo terminare
		dec ecx; decremento ecx in caso n != 0

		mov successione[ECX * 4], 1; 2nd elemento di fib è 1
		mov eax, 1
		mov ebx, 1
		cmp ecx, 0; se ho n = 1, ecx sarà 0 e dunque devo terminare
		je Scambio

		Fib : ; fibonacci iterativo
		cmp ecx, 0; controllo se sono arrivato al nth elemento
		je Scambio; se si, procedo a swappare i numeri, in quanto vengono salvati backwards
		add eax, ebx; f(n - 1) + f(n - 2)(fibonacci rule)
		mov edx, eax; salvo eax(somma) in edx
		mov eax, ebx; f(n - 1) = f(n - 2)
		mov ebx, edx; f(n - 2) = f(n)
		mov successione[ecx * 4 - 4], eax; salvo nella successione
		loop Fib

		Scambio : ; essendo salvati al contrario, devo swappare f[0..n - 1] = f[n - 1..0]
		xor ebx, ebx
		xor edx, edx; indice 0..n - 1
		mov ecx, n; indice n - 1..0
		xor eax, eax
		Scambia :
		cmp ecx, edx; se gli indici sono uguali o ecx < edx, allora termina
			jbe Fine
			mov eax, successione[ecx * 4]; salvo successione[n - 1..0] in eax
			mov ebx, successione[edx * 4]; salvo successione[0..n - 1] in ebx
			xchg eax, ebx; scambio i due elementi
			mov successione[ecx * 4], eax; salvo i due elementi nella posizioni swappate
			mov successione[edx * 4], ebx
			inc edx; incr del contatore 0..n - 1 e decremento con loop ecx
			loop Scambia
			Fine :
	}

	//Stampa su video
	{
		int i;
		for (i = 0;i <= n;i++)
		{
			printf("%d\n", successione[i]);
		}
	}
}
