#include <stdio.h>
#include <stdlib.h>

void main()
{
    // Variabili
#define MAX_LEN	100
    char frase[MAX_LEN] = "Elaborato: 3, 2004, 2";
    char parolaMax[MAX_LEN + 1];
    char parolaMin[MAX_LEN + 1];

    char trovataFine = 0;

    for (int i = 0; i < MAX_LEN; i++)
    {
        if (trovataFine == 1)
        {
            frase[i] = -1;
        }
        if (frase[i] == 0)
        {
            trovataFine = 1;
        }
    }
    for (int i = 0; i < MAX_LEN + 1; i++)
    {
        parolaMax[i] = -1;
        parolaMin[i] = -1;
    }
    // Blocco assembler
    __asm
    {
        mov parolaMax, 0 // In caso frase sia "", anche parolaMax e parolaMin, risulteranno ""
        mov parolaMin, 0
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx

        // Punta al byte precedente al primo perché poi, per comodità con il codice successivo, lo incrementerà subito.
        lea esi, frase
        dec esi

        // Puntatore all'inizio della parola attualmente di dimensione massima e sua dimensione.
        mov ecx, 0
        mov bl, 0

        // Puntatore all'inizio della parola attualmente di dimensione minima e sua dimensione.
        mov edx, 0
        mov bh, MAX_LEN

        // Ripete fino alla fine della stringa da analizzare.
    cerca:
        cmp byte ptr[edi], 0    // In caso il fine stringa venga passato con l'aggiornamento di esi successivo, controllo se edi ci è già
        je copie

        inc esi
        cmp byte ptr[esi], 0
        je copie
        // Con edi si cerca il puntatore alla fine della parola (carattere dopo all'ultimo) mentre esi punterà all'inizio della parola attualmente in analisi.
        mov edi, esi

    cerca_lunghezza:
        // Legge il carattere attuale per cercare la fine della parola attuale.
        mov al, [edi]
        cmp al, 0
        je calcola_lunghezza
        cmp al, 020h; se è uno spazio, esci dalla parola corrente(20 = spazio)
        je calcola_lunghezza
        cmp al, 02Eh; se è un segno di punteggiatura, esci dalla parola corrente(2E = .)
        je calcola_lunghezza
        cmp al, 02Ch; se è un segno di punteggiatura, esci dalla parola corrente(2c =, )
        je calcola_lunghezza
        cmp al, 03Bh; se è un segno di punteggiatura, esci dalla parola corrente(3B = ;)
        je calcola_lunghezza
        cmp al, 03Ah; se è un segno di punteggiatura, esci dalla parola corrente(3A = :)
        je calcola_lunghezza
        cmp al, 05Ch; se è un segno di punteggiatura, esci dalla parola corrente(5C = \)
        je calcola_lunghezza
        cmp al, 021h; se è un segno di punteggiatura, esci dalla parola corrente(21 = !)
        je calcola_lunghezza
        cmp al, 03Fh; se è un segno di punteggiatura, esci dalla parola corrente(3F = ? )
        je calcola_lunghezza

    avanza:
        // La parola non è terminata continua a cercarne la fine.
        inc edi
        jmp cerca_lunghezza

    calcola_lunghezza:
        // Arrivato alla fine della parola ne calcola la lunghezza come differenza tra i due puntatori.
        mov eax, edi
        sub eax, esi

        // Potrebbero esserci più caratteri terminatori di parola di fila, in tali casi si otterrebbe lunghezza = 0.
        // In tali situazioni conviene procedere con una nuova ricerca altrimenti, nella determinazione del minimo, una lunghezza uguale a 0 sarebbe per forza un minimo: il codice non deve cercare parole di lunghezza zero.
        cmp eax, 0
        je cerca

        // Verifica se ha trovato una nuova lunghezza massima.
        cmp al, bl
        jna controlla_minimo

        // Aggiorno la lunghezza della parola massima e il suo puntatore.
        mov bl, al
        mov ecx, esi

    controlla_minimo:
        // Verifica se ha trovato una nuova lunghezza minima.
        cmp al, bh
        jnb prosegue_ricerca

        // Aggiorno la lunghezza della parola minima e il suo puntatore.
        mov bh, al
        mov edx, esi

    prosegue_ricerca:
        // Procede la ricerca per la prossima parola, poi esi è incrementato di uno per non continuare a puntare al carattere di fine parola trovato precedentemente.
        mov esi, edi
        jmp cerca

    copie:
        // Se il puntatore alla parola della massima lunghezza è ancora 0 significa che non c'erano parole, ad esempio se la stringa da analizzare fosse vuota.
        cmp ecx, 0
        je fine

        // Salvo tutti i valori correnti dei registri, in questi modo posso riutilizzarli per altri scopi.
        pushad
        xor edx, edx    // Contatore.
        mov esi, ecx    // Sposto il puntatore alla parola massima in esi.

    ScriviMax:
        mov al, [esi]   // AL conterrà ogni carattere
        mov parolaMax[edx], al  // Salvo il carattere in parolaMax
        inc esi         // Incremento esi per il prossimo carattere.
        inc edx         // Incremento il contatore.
        cmp dl, bl      // Guardo se sono arrivato alla lunghezza della parola, se sono arrivato esco, altrimenti continuo a scrivere in parolaMax
        jne ScriviMax
        mov parolaMax[edx], 0 // Carattere di fine stringa.
        popad   // Recupero tutti i registri pushati.

        pushad
        xor ecx, ecx    // Contatore.
        mov esi, edx    // Sposto il puntatore alla parola minima in esi.

    ScriviMin:
        mov al, [esi]    // AL conterrà ogni carattere
        mov parolaMin[ecx], al  // Salvo il carattere in parolaMin
        inc esi         // Incremento esi per il prossimo carattere.
        inc ecx         // Incremento il contatore.
        cmp cl, bh      // Guardo se sono arrivato alla lunghezza della parola, se sono arrivato esco, altrimenti continuo a scrivere in parolaMin
        jne ScriviMin
        mov parolaMin[ecx], 0 // Carattere di fine stringa.
        popad       // Recupero tutti i registri pushati.

    fine:
    }
    // Stampa su video
    printf("%s\n%s\n%s\n", frase, parolaMax, parolaMin);
}

