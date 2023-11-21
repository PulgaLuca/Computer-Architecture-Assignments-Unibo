/********************************************************************************
 *                                                                              *
 *                   Architetture dei sistemi di Elaborazione                   *
 *                                                                              *
 ********************************************************************************

                                Elaborato 3
 Descrizione: Dato un array di BYTE, invertire l’ordine dei bit all’interno
              dell’array.

 ********************************************************************************/

#include <stdio.h>

void main()
{
#define MAX_LEN	100

    // Input
    unsigned char vet[] = { 0x01 };			//Array di BYTE
    unsigned int len = sizeof(vet) / sizeof(vet[0]);	// numero di byte in vet	
// Output
    unsigned char res[MAX_LEN];

    // Blocco assembler
    __asm
    {
        lea     esi, vet; esi punta al primo elemento del vet
        lea     edi, res
        add     edi, len
        dec     edi; edi punta all ultimo elemento di res
        mov     ebx, len

        ScambioByteInArr : ; Ciclo per scambiare i byte
        cmp ebx, 0; se ho finito gli elementi, esco
        je Fine

        mov al, [esi]; al = elemento curr del vettore puntato da esi
        mov ecx, 8; counter dei loop da effettuare
        xor dl, dl

        // Passo un bit alla volta per invertirne l'ordine e le rotazioni per passarlo in ordine inverso attraverso il carry flag
        Inversione8Bit : ; Ciclo per inversione dei bit
        rcr al, 1; l ultimo bit a destra, entra a sinistra
        rcl dl, 1; l ultimo bit a sinistra, entra a destra
        loop Inversione8Bit

        mov[edi], dl; dl(valore convertito), lo metto in res nella pos ebx

        inc esi; prossimo elemento in vet
        dec edi; n - ebx elemento in res

        dec ebx
        jmp ScambioByteInArr;

    Fine:
    }
    // Stampa su video
    {
        unsigned int i;
        for (i = 0;i < len;i++)
            printf("res[%2d] = %10d (%08X)\n", i, res[i], res[i]);
    }
}

