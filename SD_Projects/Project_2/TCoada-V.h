/*-- TCoada-V.h -- elementele cozii sunt memorate intr-un vector --*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "tlista.h"

#ifndef _COADA_VECTOR_
#define _COADA_VECTOR_

typedef struct coada
{
    size_t     dime;         /* dimensiunea unui element din coada          */
    TLista     ic;         /* adresa primului element din coada         */
    TLista     sc;         /* adresa de sfarsit a cozii                 */
} TCoadaV, *AQV;

/*- macrodefinitii - acces campuri -*/
#define IC(a)     (((AQV)(a))->ic)
#define SC(a)     (((AQV)(a))->sc)
#define DIMEQ(a) (((AQV)(a))->dime)

/*- teste si prelucrari -*/

/* verifica daca o coada este vida                                         */
#define VIDAQ(a)         (IC(a) == NULL && SC(a) ==NULL)


#endif

