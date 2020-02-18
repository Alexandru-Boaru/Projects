/*-- TStiva-V.h -- elementele stivei sunt memorate intr-un vector --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "tlista.h"


#ifndef _STIVA_VECTOR_
#define _STIVA_VECTOR_

typedef struct
{
    size_t     dime;         /* dimensiunea unui element al stivei             */
    TLista     vf;         /* varful stivei                                 */
} TStiva, *ASt;

/*- macrodefinitii - acces campuri */
#define DIME(a) (((ASt)(a))->dime)
#define VF(a)     (((ASt)(a))->vf)

/*- teste -*/

/* verifica daca o stiva este vida */
#define VIDA(a)      (VF(a) == NULL)

#endif

