#include "info.h"

TLista pop(ASt a);
void push(ASt a, TLista ae);
void rastoarna(ASt ad, ASt as);
TLista extrQ(AQV a);
void intrQ(AQV a, TLista ae);
void concatQ(AQV ad, AQV as);


void *InitS(size_t d,...)	//functie pentru initializarea stivelor
{
    ASt a = (ASt)malloc(sizeof (TStiva));            /* adresa descriptor alocat */
    if (!a)
        return NULL;
    a->dime = d;
    
    /* actualizeaza adrese din descriptor */
    a->vf = NULL;
    return (void *)a;
}


void* InitQ(size_t d,...)		//functie pentru initializarea cozilor
{
    AQV a = (AQV)malloc(sizeof(TCoadaV));      /* adresa descriptor alocat */
    if (!a)
        return NULL;
    
    
    a->dime = d;
    
    /* actualizeaza adrese din descriptor */
    a->ic = NULL;
    a->sc = NULL;
    
    return (void*)a;
}

void distrS(ASt a){		//functie care distruge stiva
    while(!VIDA(a)){
        TLista aux=pop(a);
        free(aux->info);
        free(aux);
    }
    free(a);
}

void distrQ(AQV a){		//functia care distruge coada
    while(!VIDAQ(a)){
        TLista aux=extrQ(a);
        free(aux->info);
        free(aux);
    }
    free(a);
}

TLista pop(ASt a){	//functie care extrage element din varful stivei
    if(VIDA(a))
        return NULL;
    TLista aux=VF(a);
    VF(a)=VF(a)->urm;
    return aux;
}

void push(ASt a, TLista ae){	//functie care pune element din varful stivei
    ae->urm=VF(a);
    VF(a)=ae;
}

void rastoarna(ASt ad, ASt as){		//functie care rastoarna elementele unei stive peste o stiva destinatie
    if(VIDA(as)){
        return;
    }
    TLista ae=NULL;
    while(!VIDA(as)){
        ae=pop(as);
        push(ad,ae);
    }
}

TLista extrQ(AQV a){		//functie care extrage element de la inceputul cozii
    TLista ae;
    if(IC(a)==NULL)
        return NULL;
    if(IC(a)==SC(a)){
        ae=IC(a);
        IC(a)=NULL;
        SC(a)=NULL;
        return ae;
    }
    ae=IC(a);
    IC(a)=IC(a)->urm;
    return ae;
}


void intrQ(AQV a, TLista ae){		//functie care pune element la sfarsitul cozii
    if(SC(a)==NULL){
        IC(a)=ae;
        SC(a)=ae;
    }
    else{
        SC(a)->urm=ae;
        SC(a)=SC(a)->urm;
    }
}



void concatQ(AQV ad, AQV as){		//functie care concateneaza elementele unei cozi la o coada destinatie
    if(VIDAQ(as))
        return;
    TLista ae=NULL;
    while(!VIDAQ(as)){
        ae=extrQ(as);
        intrQ(ad,ae);
    }
}


void AddH(ALista h, char* cul_disc){		//functie care adauga un nou sistem Hanoi

    TLista H=*h;
    TLista aux=malloc(sizeof(struct celula));
    aux->info=malloc(sizeof(struct hanoi));
    aux->urm=NULL;
    ((THanoi)aux->info)->NrDisc=0;

    ((THanoi)aux->info)->culoare=malloc(20);
    strcpy(((THanoi)aux->info)->culoare,cul_disc);


    ((THanoi)aux->info)->A=InitS(sizeof(struct disc));
    ((THanoi)aux->info)->B=InitS(sizeof(struct disc));
    ((THanoi)aux->info)->C=InitS(sizeof(struct disc));
    ((THanoi)aux->info)->comenzi=InitQ(sizeof(struct comanda));
    ((THanoi)aux->info)->check=0;

    if((*h)==NULL){		
        *h=aux;

    }
    else{
        for(;H->urm!=NULL;H=H->urm);
        H->urm=aux;

    }
}

void InsOrd(ASt *A,int dim_disc){		//functie pentru inserarea ordoanata in stiva A a sistemului Hanoi

    TLista aux=malloc(sizeof(struct celula));
    aux->info=malloc(sizeof(struct disc));
    ((TDisc)aux->info)->diametru=dim_disc;
    if(VIDA(*A)){
        push((*A),aux);

        return;
    }
    ASt saux=InitS((sizeof(struct disc)));
    TLista s=malloc(sizeof(struct celula));
    s->info=malloc(sizeof(struct disc));

    while (!VIDA((*A)))		//folosind pop succesiv, se cauta locul discului in stiva
    {
        s=pop((*A));

        
        if(((TDisc)s->info)->diametru<dim_disc){
            push(saux,s);

        }
        else{

            push(saux, aux);
            push(saux, s);
            rastoarna((*A), saux);
            return;
        }
    }

    push(saux,aux);
    rastoarna((*A), saux);
}

void add(ALista aS, char* cul_disc, int dim_disc){	//functie prin care se adauga disc intr-un turn A dintr-un sistem Hanoi
    int n=0;

    TLista H=malloc(sizeof(struct celula));
    H = ((TSistem)(*aS)->info)->Hanoi;


    for(;H!=NULL;H=H->urm){

        if(strcmp(((THanoi)H->info)->culoare,cul_disc)==0){	//cauta sistemul care are culoarea corespunzatoare
            n=1;
            break;
        }
    }
    H=((TSistem)(*aS)->info)->Hanoi;
    ASt S=InitS(sizeof(struct disc));
    if(H==NULL || n==0){		//daca nu exista inca nici un sistem sau daca nu exista sistemul cu culoarea ceruta
					//se foloseste AddH pentru a crea noul sistem
        AddH(&H,cul_disc);
        TLista H1=H;
        for(;H!=NULL;H=H->urm){


            if(strcmp(((THanoi)H->info)->culoare,cul_disc)==0){
                break;
            }
        }
        S=((THanoi)H->info)->A;

        InsOrd(&S,dim_disc);
        ((THanoi)H->info)->NrDisc++;

        
        ((TSistem)(*aS)->info)->Hanoi=H1;

    }
    else{
        for(;H!=NULL;H=H->urm){		
            if(strcmp(((THanoi)H->info)->culoare,cul_disc)==0){
                S=((THanoi)H->info)->A;	
                InsOrd(&S,dim_disc);			//inserez noul disc
                ((THanoi)H->info)->NrDisc++;		//creste numarul de discuri
                n=1;
                break;
            }
        }
    }
}

void towerOfHanoi(AQV q, int n, char from_rod, char to_rod, char aux_rod) 
{   
   //functie recursiva prin care se rezolva mutarile jocului Hanoi
    
    if (n == 1)
    {
       
        TLista aux=malloc(sizeof(struct celula));
        aux->info=malloc(sizeof(struct comanda));
        ((TComanda)aux->info)->numeComanda=malloc(5);
        ((TComanda)aux->info)->numeComanda[0]=from_rod;
        ((TComanda)aux->info)->numeComanda[1]='-';
        ((TComanda)aux->info)->numeComanda[2]='>';
        ((TComanda)aux->info)->numeComanda[3]=to_rod;

        intrQ(q, aux);

        return;
    }
    towerOfHanoi(q, n-1, from_rod, aux_rod, to_rod);

   
    TLista aux=malloc(sizeof(struct celula));
    aux->info=malloc(sizeof(struct comanda));
    ((TComanda)aux->info)->numeComanda=malloc(5);
    ((TComanda)aux->info)->numeComanda[0]=from_rod;
    ((TComanda)aux->info)->numeComanda[1]='-';
    ((TComanda)aux->info)->numeComanda[2]='>';
    ((TComanda)aux->info)->numeComanda[3]=to_rod;
    intrQ(q, aux);
    towerOfHanoi(q, n-1, aux_rod, to_rod, from_rod);
}


void play(ALista aS, char* culoare, int nrMut){			//functie prin care se fac mutarile in jocul Hanoi

    TLista H = ((TSistem)(*aS)->info)->Hanoi;

    for(;H!=NULL;H=H->urm){
        if(strcmp(((THanoi)H->info)->culoare,culoare)==0){	//cauta sistemul care are culoarea corespunzatoare
            break;
        }
    }

    if(((THanoi)H->info)->check==0){		//verifica daca a fost creata coada de comenzi a sistemului, in caz contrar, se creaza
        int n=((THanoi)H->info)->NrDisc;	//coada folosind towerOfHanoi
	AQV q=InitQ(sizeof(struct comanda));
        q=((THanoi)(H)->info)->comenzi;
        towerOfHanoi(q, n,'A','C','B');
        ((THanoi)H->info)->check=1;
    }

    AQV com=((THanoi)H->info)->comenzi;
    ASt A=((THanoi)H->info)->A;
    ASt B=((THanoi)H->info)->B;
    ASt C=((THanoi)H->info)->C;
    int i=0;
    TLista aux=malloc(sizeof(struct celula));
    aux->info=malloc(sizeof(struct comanda));

    for(;i<nrMut;i++){		//executa numarul de mutari cerut

        ASt to = InitS(sizeof(struct disc));
        ASt fr = InitS(sizeof(struct disc));
        aux=extrQ(com);
   
        char *coma = ((TComanda)aux->info)->numeComanda;	//string in care retin comanda

        switch (coma[0]) {	//identific stiva din care scot discul
            case 'A':
                fr=A;
                break;
            case 'B':
                fr=B;
                break;
            case 'C':
                fr=C;
                break;
        }
        switch (coma[3]) {	//identific stiva in care pun discul
            case 'A':
                to=A;
                break;
            case 'B':
                to=B;
                break;
            case 'C':
                to=C;
                break;
        }
        TLista ae=malloc(sizeof(struct celula));
        ae->info=malloc(sizeof(struct disc));
        ae=pop(fr);
					//se executa mutarea
        push(to,ae);

        if(VIDAQ(com)){			//daca se termina jocul se iese din iteratie
            break;
        }
    }
}




void show(ALista aS, char* culoare, FILE *out){		//functie prin care se afiseaza cum sunt pozitionate discurile in turnuri
    int n=0;


    TLista H = ((TSistem)(*aS)->info)->Hanoi;
    for(;H!=NULL;H=H->urm){
        if(strcmp(((THanoi)H->info)->culoare,culoare)==0){	//cauta sistemul care are culoarea corespunzatoare
            n=1;
            break;
        }
    }
    if(n==0){				//daca nu exista inca niciun disc cu acea culoare, printez sistemul ca fiind gol
        fprintf(out,"A_%s:\n",culoare);
        fprintf(out,"B_%s:\n",culoare);
        fprintf(out,"C_%s:\n",culoare);
        return;
    }
    

    ASt A=((THanoi)H->info)->A;
    ASt B=((THanoi)H->info)->B;
    ASt C=((THanoi)H->info)->C;


    ASt saux=InitS(sizeof(struct disc));	//stiva auxiliara
    TLista ae=NULL;
    fprintf(out,"A_%s:",culoare);
    
    rastoarna(saux, A);


    while(!VIDA(saux)){		//printez discurile din turnul A
        ae=pop(saux);
        fprintf(out," %d",((TDisc)ae->info)->diametru);
        push(A,ae);
    }


    fprintf(out,"\n");
    fprintf(out,"B_%s:",culoare);
    rastoarna(saux, B);
    while(!VIDA(saux)){		//printez discurile din turnul B
        ae=pop(saux);
        fprintf(out," %d",((TDisc)ae->info)->diametru);
        push(B,ae);
    }
    fprintf(out,"\n");
    fprintf(out,"C_%s:",culoare);
    rastoarna(saux, C);
    while(!VIDA(saux)){		//printez discurile din turnul C
        ae=pop(saux);
        fprintf(out," %d",((TDisc)ae->info)->diametru);
        push(C,ae);
    }
    fprintf(out,"\n");
    
}


void show_moves(ALista aS, char* culoare, int nrMut, FILE *out){	//functie prin care se afiseaza urmatoarele miscari
    TLista H = ((TSistem)(*aS)->info)->Hanoi;
    for(;H!=NULL;H=H->urm){				//cauta sistemul care are culoarea corespunzatoare
        if(strcmp(((THanoi)H->info)->culoare,culoare)==0){	
            break;
        }
    }
    if(((THanoi)H->info)->check==0){		//verifica daca a fost creata coada de comenzi a sistemului, in caz contrar, se creaza 
        int n=((THanoi)H->info)->NrDisc;	//coada folosind towerOfHanoi
	AQV q=InitQ(sizeof(struct comanda));
        q=((THanoi)(H)->info)->comenzi;
        towerOfHanoi(q, n,'A','C','B');
        ((THanoi)H->info)->check=1;
    }
    AQV com=((THanoi)H->info)->comenzi;		//com e coada ce contine instructiunile
    fprintf(out,"M_%s:",culoare);

    if(VIDAQ(com)){		//daca coada e goala se iese din functie
	fprintf(out,"\n");
	return;
    }
    AQV caux=InitQ(sizeof(struct comanda));		//coada auxiliara
    TLista ae=malloc(sizeof(struct celula));		//element auxiliar
    ae->info=malloc(sizeof(struct comanda));
    int i=0;
    for(; i<nrMut; i++){		//folosind extrQ, scot instructiunile din com, le printez si le pun in coada auxiliara

        ae=extrQ(com);
        fprintf(out," %s",((TComanda)ae->info)->numeComanda);		
        intrQ(caux, ae);

        if(VIDAQ(com)){
            break;
        }
    }
    concatQ(caux, com);		//concatenez ce a ramas din com in caux si apoi concatenez caux in com
    concatQ(com, caux);
    fprintf(out,"\n");
}





int main(int argc, char*argv[])
{
    int N=0,i=0;
    FILE *in=fopen(argv[1],"rt");
    fscanf(in, "%d", &N);
    TLista s=malloc(sizeof(struct celula));
    s->info=malloc(sizeof(struct sistem));
    ((TSistem)s->info)->Hanoi=NULL;
    FILE *out=fopen(argv[2],"wt");	//deschiderea fisierului pentru output
    for(;i<N;i++){		
        char* com=malloc(20);
        fscanf(in,"%s",com);		//citerea instructiunilor
        if(strcmp(com,"add")==0){	//se executa add
            char* cul_disc=malloc(20);
            int dim_disc;
            fscanf(in,"%s %d",cul_disc,&dim_disc);
            add(&s,cul_disc,dim_disc);
        }
        if(strcmp(com,"play")==0){	//se executa play
            char* culoare=malloc(20);
            int nr_mut;
            fscanf(in,"%s %d",culoare,&nr_mut);
            play(&s,culoare,nr_mut);

        }
        if(strcmp(com,"show")==0){	//se executa show
            char* culoare=malloc(20);
            fscanf(in,"%s",culoare);
            show(&s,culoare,out);
        }
        if(strcmp(com,"show_moves")==0){	//se executa show_moves
            char* culoare=malloc(20);
            int nr_mut;
            fscanf(in,"%s %d",culoare,&nr_mut);
            show_moves(&s, culoare, nr_mut,out);

        }
    }
    fclose(out);		//se inchid fisierele de input si output
    fclose(in);
    TLista H = ((TSistem)s->info)->Hanoi;
    while(H->urm!=NULL){			//se distrug structurile de date: stivele, cozile, listele
        ASt A=((THanoi)H->info)->A;
        ASt B=((THanoi)H->info)->B;
        ASt C=((THanoi)H->info)->C;
        AQV com=((THanoi)H->info)->comenzi;
        distrS(A);
        distrS(B);
        distrS(C);
        distrQ(com);
        free(((THanoi)H->info)->culoare);
        TLista aux=H;
        H=H->urm;
        free(aux);
    }
    free(H);
    free(s);
    
    return 0;
}


