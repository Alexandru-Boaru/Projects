/*--- tarb.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

typedef struct nod {
    char* info;		//informatia din arbore este salvata ca char*
    struct nod *st, *dr;
} TNod, *TArb, **AArb;

#endif

