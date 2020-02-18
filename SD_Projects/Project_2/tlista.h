/*-- tlista.h --- LISTA SIMPLU INLANTUITA cu elemente de tip intreg ---*/

#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{
    void* info;
    struct celula * urm;
} TCelula, *TLista, **ALista; /* tipurile Celula, Lista si Adresa_Lista */



#endif


