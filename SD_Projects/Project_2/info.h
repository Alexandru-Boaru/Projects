//
//  info.h
//  
//
#include "TStiva-V.h"
#include "TCoada-V.h"
#include "tlista.h"

#ifndef info_h
#define info_h

typedef struct disc
{
    int diametru;       /* diametrul discului             */
}*TDisc;
typedef struct hanoi{
    ASt A;       /* stiva A in care se pun discurile initial             */
    ASt B;       /* stiva B auxiliara            */
    ASt C;       /* stiva C in care trebuie sa ajunga toate discurile             */
    AQV comenzi;
    char* culoare;      /* culoarea discurilor             */
    int NrDisc;         /* numarul de discuri din acest sistem Hanoi             */
    int check;
}*THanoi;

typedef struct sistem{
    TLista Hanoi;       /* lista sistemelor Hanoi             */
}*TSistem;

typedef struct comanda{
    char* numeComanda;
}*TComanda;

#endif /* info_h */

