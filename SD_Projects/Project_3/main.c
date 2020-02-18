#include "tarb.h"
#include "tlista.h"

void DistrugeL(ALista aL)			//functie pentru ditrugerea listei
{
	TLista aux;
  	while(*aL){
		aux = *aL;
    		*aL = aux->urm;
		free(((TVar)aux->info)->nume);	
    		free(aux->info);		//distruge informatia din celula
    		free(aux);			//distruge celula
  	}
}


void distruge(TArb r)     // functie auxiliara - distruge toate nodurile 
{
	if (!r) return;
    	distruge (r->st);     // distruge subarborele stang 
    	distruge (r->dr);     // distruge subarborele drept 
    	free (r->info);	      // distruge informatia din radacina 
    	free (r);             // distruge nodul radacina 
}


void DistrArb(TArb *a) // distruge toate nodurile arborelui de la adresa a 
{
    	if (!(*a)) return;       // arbore deja vid 
    	distruge (*a);           // distruge toate nodurile 
    	*a = NULL;               // arborele este vid 
}


TArb ConstrFr(char* x)     // -> adresa frunza cu informatia x, sau
                           // NULL daca nu exista spatiu 
{
    	TArb aux = (TArb)malloc (sizeof(TNod));
    	if (!aux) return NULL;
    	aux->info=malloc(10);	//aloca memorie pentru informatia din nod
    	strcpy(aux->info,x); aux->st = aux->dr = NULL;
    	return aux;
}


int Inserare(TArb* a, char* p, int n){		// functie recursiva de inserare a nodurilor in arbore
    	if(p==NULL) return 1;			//se iese din functie daca s-au citit toate informatiile din expresie
    	if(p[strlen(p)-1]=='\n'){
        	p[strlen(p)-1]='\0';		//inlocuiesc caracterul de EOL cu caracterul de sfarsit de linie daca e cazul
    	}
    	(*a)=ConstrFr(p);			//construiesc un nod cu informatia data
    	if(p[0]=='+' || (p[0]=='-' && isdigit(p[1])==0)  || p[0]=='*' || p[0]=='/' || p[0]=='^'){	
        	p=strtok(NULL, " ");
        	Inserare(&(*a)->st,p, n+1);	//insereaza subarbore stang
        	p=strtok(NULL, " ");
        	Inserare(&(*a)->dr,p, n+1);	//insereaza subarbore drept
    	}
   	else{
        	if(strcmp(p,"sqrt")==0){	//daca operatorul din radacina este "sqrt" insereaza numai un subarbore stang
            		p=strtok(NULL, " ");
            		Inserare(&(*a)->st,p, n+1);	
        	}
    	}
    	return 1;
}


int Rezolvare(TArb a, TLista s, int* v, FILE *out){	//functie pentru calcularea expresiei parcurgand arborele in RSD
    	int r = 0,x = 0,y = 0,t=0;
    	if(a->st){
        	x=Rezolvare(a->st,s,v,out);		//se rezolva expresia din subarborele stang
    	}
    	if(a->dr){
        	y=Rezolvare(a->dr,s,v,out);		//se rezolva expresia din subarborele drept
    	}

    	if(((a->info)[0]>='0' && (a->info)[0]<='9') || ((a->info)[1]>='0' && (a->info)[1]<='9' && (a->info)[0]=='-')){
        	sscanf(a->info, "%d", &r);		//daca operandul este de tip numeric il convertesc in int
    	}
    	if(strcmp(a->info,"+")==0){			//adunare "+"
        	r=x+y;
    	}
    	if(strcmp(a->info,"-")==0){			//scadere "-"
    		r=x-y;
    	}
    	if(strcmp(a->info,"*")==0){			//inmultire "*"
        	r=x*y;
    	}
    	if(strcmp(a->info,"/")==0){			//impartire "/"
        	if(y==0){
            		if((*v)==0){
                		fprintf(out,"Invalid expression\n");	//daca impartitorul este 0, printeaza eroare
            		}
            		(*v)=1;
            		return 1;
        	}
        	else{
        		r=x/y;
        	}
    	}
    	if(strcmp(a->info,"^")==0){			//ridicare la putere "^"
        	r=pow(x,y);
    	}
    	if(strcmp(a->info,"sqrt")==0){			//radical de ordinul 2
        	if(x<0){
            		if((*v)==0){
                		fprintf(out,"Invalid expression\n");	//daca numarul este negativ, printeaza eroare
            		}
            		(*v)=1;
        	}
        	else{
            		return floor(sqrt(x));
        	}
    	}
    	if((a->info)[0]>='a' && (a->info)[0]<='z'){	//daca operandul este o variabila
        	TLista aux=s;				//se cauta in lista de variabile operandul dorit
        	while(aux){
            		if(strcmp(((TVar)aux->info)->nume,a->info)==0){
                		r=((TVar)aux->info)->val;
                		t=1;
                		break;
            		}
            		aux=aux->urm;
        	}
        	if(t==0 && (*v)==0){
            		fprintf(out,"Variable %s undeclared\n",a->info);	// daca variabila nu exista in lista, printeaza eroare
            		(*v)=1;
        	}
    	}
    	return r;
}

TArb ConstrNew(char* exp){	//functie pentru construirea unui nou arbore
    	TArb arb;
    	char *p;
    	p=strtok(exp," ");    
    	int nr=0;
    	Inserare(&arb,p,nr);	//se inseraza arborele
    	return arb;
}


int main (int argc, char*argv[])
{
    	TLista s=NULL;		//initializare lista pentru variabile
    	int N=0,M=0,i=0,j=0;
    	FILE *in=fopen(argv[1],"rt");	//se deschide fisierul cu data de intrare
    	fscanf(in,"%d",&N);		//scaneaza numarul de variabile
    	for(;i<N;i++){
        	char* com=malloc(10);
        	char* eg=malloc(1);
        	int val=0;
        	fscanf(in,"%10s",com);
        	fscanf(in,"%s",eg);
        	fscanf(in,"%d",&val);

        	TLista aux=malloc(sizeof(struct celula));	//aloca celule pentru lista
        	aux->info=malloc(sizeof(struct variabila));
        	aux->urm=NULL;
        	((TVar)aux->info)->val=val;
        	((TVar)aux->info)->nume=malloc(strlen(com));

        	strcpy(((TVar)aux->info)->nume,com);

       		if(!s){
            		s=aux;
        	}
        	else{
            		TLista S=s;
            		for(;S->urm!=NULL;S=S->urm);
            		S->urm=aux;
        	}
		free(com);
        	free(eg);
    	}

    	fscanf(in,"%d",&M);			//scaneaza numarul de expresii
    	char* exp=malloc(300);
    	int v=0;
    	int r=0;
    	FILE *out=fopen(argv[2],"wt");		//se deschide fisierul de output
    	fgets(exp,2,in);
    	for(;j<M;j++){
        	fgets(exp,300,in);		//se scaneaza expresia
        	TArb arb;
        	arb=ConstrNew(exp);		//se construieste un nou arbore
        	r=Rezolvare(arb,s,&v,out);	//se rezolva expresia din arbore
        	if(v==0){
            		fprintf(out,"%d\n",r);	//printeaza rezultatul obtinut daca nu au existat erori
        	}
        	v=0;
        	DistrArb(&arb);			//distruge arborele de expresie
    	}

    	DistrugeL(&s);		//distruge lista cu variabile
    	fclose(in);		//inchide fisierul de input
    	fclose(out);		//inchide fisierul de output
    	free(exp);
    	return 0;
}

