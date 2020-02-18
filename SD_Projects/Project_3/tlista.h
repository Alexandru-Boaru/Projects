
#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{
    void* info;
    struct celula * urm;
} TCelula, *TLista, **ALista; // tipurile Celula, Lista si Adresa_Lista 

typedef struct variabila	//structura pentru variabile
{
    char* nume;		//numele variabilei
    int val;		//valoarea variabilei
}*TVar;

#endif

