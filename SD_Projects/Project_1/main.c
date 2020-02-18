/*BOARU Alexandru-311CB */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "info.h"

void PrintJ(TLista L,FILE *out){	//printeaza jucatorii

	L=L->urm;
    
	for(;L->info!=NULL;L=L->urm){
        	if(L->urm->info==NULL){
            		fprintf(out," %s - %d.",((Jucator)L->info)->nume,((Jucator)L->info)->nrMaini);
            	break;
        	}
        fprintf(out," %s - %d;",((Jucator)L->info)->nume,((Jucator)L->info)->nrMaini);
    	}
}

void PrintM(TLista L,FILE *out){		//apeleaza lista de jucatori de la fiecare masa si printeaza mesele

	if(!L) fprintf(out,"Sala este inchisa!");
    	for(;L != NULL; L = L->urm){
        	fprintf(out,"%s:",((Masa)L->info)->numeMasa);
        	PrintJ(((Masa)L->info)->jucatori,out);
        	fprintf(out,"\n");
	}
}

void Noroc(ALista aL, char* nume_masa, char* nume_jucator, int grad_noroc, FILE *out){		//mareste norocul jucatorului nume_jucator
	TLista L=(*aL);										//de la masa nume_masa cu grad_noroc
    	int n1=0,n2=0;										//unitati
    	for(;L!=NULL;L=L->urm){
       		if(strcmp(((Masa)L->info)->numeMasa,nume_masa)==0){
            	n1=1;
            	TLista J=((Masa)L->info)->jucatori;
            	J=J->urm;
            	for(;J->info!=NULL;J=J->urm){
                	if(strcmp(((Jucator)J->info)->nume,nume_jucator)==0){
                    		n2=1;
                    		((Jucator)J->info)->nrMaini=((Jucator)J->info)->nrMaini+grad_noroc;
                		}
            		}
        	}
    	}
if(n1==0) fprintf(out,"Masa %s nu exista!\n",nume_masa);
if(n1==1 && n2==0) fprintf(out,"Jucatorul %s nu exista la masa %s!\n",nume_jucator,nume_masa);    
}

void Ghinion(ALista aL,char* nume_masa, char* nume_jucator, int grad_ghinion, FILE *out){	//scade norocul jucatorului nume_jucator
    	TLista M=((Sala)(*aL)->info)->masa;							//de la masa nume_masa cu grad_ghinion
    	int n1=0,n2=0;										//unitati

    	if(strcmp(((Masa)M->info)->numeMasa,nume_masa)==0){
        	n1=1;
        	TLista J=((Masa)M->info)->jucatori;
        	while(J->urm->info!=NULL){
            		if(strcmp(((Jucator)J->urm->info)->nume,nume_jucator)==0){
				n2=1;
                		((Jucator)J->urm->info)->nrMaini=((Jucator)J->urm->info)->nrMaini-grad_ghinion;
                		if(((Jucator)J->urm->info)->nrMaini<=0){
                    			TLista aux=J->urm;
                    			J->urm=aux->urm;
                    			free(aux->info);
                    			free(aux);
                    			((Masa)M->info)->nrCrtJucatori--;
                    			((Sala)(*aL)->info)->nrLocCrt--;
                		}
                		break;

            		}
            		else{
                		J=J->urm;
            		}            
        	}
        	if(((Masa)M->info)->nrCrtJucatori<=0){
            		((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->info)->nrMaxJucatori;
            		TLista aux=((Sala)(*aL)->info)->masa;
            		((Sala)(*aL)->info)->masa=((Sala)(*aL)->info)->masa->urm;
            		free(aux->info);
            		free(aux);
            		((Sala)(*aL)->info)->nrMese--;
        	}
    	}
    	else{
    		while(M->urm!=NULL){
        		if(strcmp(((Masa)M->urm->info)->numeMasa,nume_masa)==0){
           			n1=1;
            			TLista J=((Masa)M->urm->info)->jucatori;
            			while(J->urm->info!=NULL){
                			if(strcmp(((Jucator)J->urm->info)->nume,nume_jucator)==0){
                    				n2=1;
                    				((Jucator)J->urm->info)->nrMaini=((Jucator)J->urm->info)->nrMaini-grad_ghinion;
                    					if(((Jucator)J->urm->info)->nrMaini<=0){
                        				TLista aux=J->urm;
                        				J->urm=aux->urm;
                        				free(aux->info);
                        				free(aux);
                        				((Masa)M->urm->info)->nrCrtJucatori--;
                        				((Sala)(*aL)->info)->nrLocCrt--;
                    					}
                    				break;
               		 		}
                			else{
                    				J=J->urm;
                			}
            			}
            			if(((Masa)M->urm->info)->nrCrtJucatori<=0){
                			((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->urm->info)->nrMaxJucatori;
                			TLista aux=M->urm;
               				if(M->urm->urm==NULL){
                    				M->urm=NULL;
                			}
                			else{
                    				M->urm=aux->urm;
                			}
                			free(aux->info);
                			free(aux);
                			((Sala)(*aL)->info)->nrMese--;
            			}
            			break;
        		}
        	M=M->urm;
    		}
	}
	if(n1==0) fprintf(out,"Masa %s nu exista!\n",nume_masa);
 	if(n1==1 && n2==0) fprintf(out,"Jucatorul %s nu exista la masa %s!\n",nume_jucator,nume_masa);
}


void Tura(ALista aL, char* nume_masa,FILE *out){	//face rotatia jucatorilor la masa nume_masa si decrementeaza cu cate o unitate
    	TLista L=*aL;					//numarul de maini de la fiecare jucator
    	TLista M=((Sala)L->info)->masa;
    	TLista J;
    	int n=0;
    	if(strcmp(((Masa)M->info)->numeMasa,nume_masa)==0){
        	n=1;
        	J=((Masa)M->info)->jucatori;
        	if(J!=J->urm->urm){
            		TLista c=J->urm;
            		TLista end=J;
            		for(;end->urm->info!=NULL;end=end->urm);
            		end->urm=J->urm;
            		J->urm=c->urm;
            		c->urm=J;
        	}
		while(J->urm->info!=NULL){
               		((Jucator)J->urm->info)->nrMaini--;
               		if(((Jucator)J->urm->info)->nrMaini<=0){
                		TLista aux=J->urm;
                    		J->urm=aux->urm;
                            free(aux->info);
                            free(aux);
                    		((Masa)M->info)->nrCrtJucatori--;
                    		((Sala)L->info)->nrLocCrt--;
                	}
                	else{
                    		J=J->urm;
                	}        
        	}
        	if(((Masa)M->info)->nrCrtJucatori<=0){
            		((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->info)->nrMaxJucatori;
           		TLista aux=((Sala)(*aL)->info)->masa;
            		((Sala)(*aL)->info)->masa=((Sala)(*aL)->info)->masa->urm;
			free(aux->info);
            		free(aux);
            		((Sala)(*aL)->info)->nrMese--;
        	}        
    	}
    	for(;M->urm!=NULL;M=M->urm){
        	if(strcmp(((Masa)M->urm->info)->numeMasa,nume_masa)==0){
            		n=1;
            		J=((Masa)M->urm->info)->jucatori;
            		if(J!=J->urm->urm){
                		TLista c=J->urm;
                		TLista end=J;
                		for(;end->urm->info!=NULL;end=end->urm);
                		end->urm=J->urm;
                		J->urm=c->urm;
                		c->urm=J;
            		}            
            		while(J->urm->info!=NULL){
                		((Jucator)J->urm->info)->nrMaini--;
                		if(((Jucator)J->urm->info)->nrMaini<=0){
                    			TLista aux=J->urm;
                    			J->urm=aux->urm;
					free(aux->info);
                    			free(aux);
                    		((Masa)M->urm->info)->nrCrtJucatori--;
                    		((Sala)L->info)->nrLocCrt--;
                		}
                		else{
                    			J=J->urm;
                		}                
            		}
            		if(((Masa)M->urm->info)->nrCrtJucatori<=0){
                		((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->urm->info)->nrMaxJucatori;
                		TLista aux=malloc(sizeof(struct celulag));
                		aux->urm=NULL;
                		aux=M->urm;
                		if(M->urm->urm==NULL){
                    			M->urm=NULL;
                    			((Sala)(*aL)->info)->nrMese--;
                                	break;
                  		}
				else{
                    			M->urm=aux->urm;
                		}
				free(aux->info);
                		free(aux);
                		((Sala)(*aL)->info)->nrMese--;
            		}
        	}
    	}
    	if(n==0) fprintf(out,"Masa %s nu exista!\n",nume_masa);

}

void Tura_completa(ALista aL){			//face rotatia juactorilor la fiecare masa, decrementand cu cate o unitate nr de maini
    	TLista L=*aL;
    	TLista M=((Sala)L->info)->masa;
    	TLista J;
    	J=((Masa)M->info)->jucatori;
    	if(J!=J->urm->urm){
        	TLista c=J->urm;
        	TLista end=J;
        	for(;end->urm->info!=NULL;end=end->urm);
        	end->urm=J->urm;
        	J->urm=c->urm;
       		c->urm=J;
    	}
	while(J->urm->info!=NULL){
                ((Jucator)J->urm->info)->nrMaini--;
                if(((Jucator)J->urm->info)->nrMaini<=0){
                    	TLista aux=J->urm;
                    	J->urm=aux->urm;
			free(aux->info);
                    	free(aux);
                    	((Masa)M->info)->nrCrtJucatori--;
                    	((Sala)L->info)->nrLocCrt--;
                }
                else{
                    	J=J->urm;
                }
        }
    	if(((Masa)M->info)->nrCrtJucatori<=0){
        	((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->info)->nrMaxJucatori;
        	TLista aux=((Sala)(*aL)->info)->masa;
     		((Sala)(*aL)->info)->masa=((Sala)(*aL)->info)->masa->urm;
		free(aux->info);
        	free(aux);
        	((Sala)(*aL)->info)->nrMese--;
    	}
    	while(M->urm!=NULL){
        	J=((Masa)M->urm->info)->jucatori;
        	if(J!=J->urm->urm){
            		TLista c=J->urm;
            		TLista end=J;
            		for(;end->urm->info!=NULL;end=end->urm);
            		end->urm=J->urm;
            		J->urm=c->urm;
            		c->urm=J;
        	}
      		while(J->urm->info!=NULL){
            		((Jucator)J->urm->info)->nrMaini--;
            		if(((Jucator)J->urm->info)->nrMaini<=0){
                		TLista aux=J->urm;
                		J->urm=aux->urm;
				free(aux->info);
                		free(aux);
                		((Masa)M->urm->info)->nrCrtJucatori--;
               		 	((Sala)L->info)->nrLocCrt--;
            		}
            		else{
                		J=J->urm;
            		}            
 		}
        	if(((Masa)M->urm->info)->nrCrtJucatori<=0){
	    		((Sala)(*aL)->info)->nrLocMax=((Sala)(*aL)->info)->nrLocMax-((Masa)M->urm->info)->nrMaxJucatori;
            		TLista aux=malloc(sizeof(struct celulag));
            		aux->urm=NULL;
            		aux=M->urm;
            		if(M->urm->urm==NULL){
                		M->urm=NULL;
                		((Sala)(*aL)->info)->nrMese--;
                		break;   
            		}
            		else{
               			M->urm=aux->urm;
            		}
            		free(aux);
            		((Sala)(*aL)->info)->nrMese--;
        	}
		else{
			M=M->urm;
		}
    	}    
}

int Comp(Jucator a, Jucator b){		//functie de comaparatie pentru InsOrd
    	int r = b->nrMaini-a->nrMaini;
    	if(r!=0) return r;
    	return -strcmp(a->nume,b->nume);
    	return 1;
}

void InsOrd(ALista aL, TLista aux){		//inseraza ordonat elementele in lista noua
    	TLista u;
    	TLista L=*aL;
    	if(L==NULL){
        	(*aL)=aux;
    	}
    	else{
        	if(Comp((Jucator)(*aL)->info,(Jucator)aux->info)>=0){
            	aux->urm=*aL;
            	*aL=aux;
        	}
        	else{
            		u=*aL;
            		while(u->urm && Comp(u->urm->info,aux->info)<0){                
                		u=u->urm;
            		}
            		aux->urm=u->urm;
            		u->urm=aux;
        	}
    	}
}

void Clasament(ALista aL, char* nume_masa, FILE *out){	//copiaza elementele listei in alta lista si apeleaza functia de inserare ordonata
    	TLista L=*aL;
    	TLista M=((Sala)L->info)->masa;
    	TLista J=NULL;
    	int n=0;
    	for(;M!=NULL;M=M->urm){
        	if(strcmp(((Masa)M->info)->numeMasa,nume_masa)==0){
        		n=1;
            		J=((Masa)M->info)->jucatori;
            		TLista R=NULL;
            		J=J->urm;
            		TLista aux;
            
            		for(;J->info!=NULL;J=J->urm){
                		aux=malloc(sizeof(Celula));
                		aux->info=J->info;
                		aux->urm=NULL;
                		InsOrd(&R,aux);
            		}
            		fprintf(out,"Clasament %s:\n",((Masa)M->info)->numeMasa);
            		for(;R!=NULL;R=R->urm){
                		fprintf(out,"%s %d\n",((Jucator)R->info)->nume,((Jucator)R->info)->nrMaini);
            		}
        	}
    	}
    	if(n==0){
        	fprintf(out,"Masa %s nu exista!\n",nume_masa);
    	}
}


void Inchide(ALista aL, char* nume_masa, FILE *out){	//inchide o masa daca sunt destule locuri in sala si muta jucatori mesei la alte
    	TLista L=*aL;					//mese
    	TLista I=((Sala)L->info)->masa;
 	TLista M=((Sala)L->info)->masa;
    	TLista M1=((Sala)L->info)->masa;
    	int n=0;
    	int m=0;
    	while(M1!=NULL){
        	if(strcmp(((Masa)M1->info)->numeMasa,nume_masa)==0){
            	n=1;
            	break;
        	}
        	M1=M1->urm;
    	}
	if(n==0){
        	fprintf(out,"Masa %s nu exista!\n",nume_masa);
		return;
    	}
    	if(((Sala)L->info)->nrLocMax - ((Masa)M1->info)->nrMaxJucatori >= ((Sala)L->info)->nrLocCrt){
       		m=1;
        	TLista aux=NULL;
        	while(((Masa)M1->info)->nrCrtJucatori != 0){
            		if(M != M1){
                		if(((Masa)M->info)->nrCrtJucatori<((Masa)M->info)->nrMaxJucatori){
                    			TLista J=((Masa)M->info)->jucatori;
                   			while(J->urm->info!=NULL){
                        				J=J->urm;
                    			}
                    			aux=((Masa)M1->info)->jucatori;
                    			while(((Masa)M->info)->nrCrtJucatori<((Masa)M->info)->nrMaxJucatori &&
											((Masa)M1->info)->nrCrtJucatori>0){
                        			TLista a=aux->urm;
                        			aux->urm=a->urm;
                        			a->urm=J->urm;
                        			J->urm=a;
                        			J=J->urm;
                        			((Masa)M->info)->nrCrtJucatori++;
                        			((Masa)M1->info)->nrCrtJucatori--;
                    			}
                		}
            		}
            		M=M->urm;
        	}
		((Sala)L->info)->nrLocMax=((Sala)L->info)->nrLocMax-((Masa)M1->info)->nrMaxJucatori;
        	if(M1==I){
       		     	TLista aux1=((Sala)(*aL)->info)->masa;
        	    	((Sala)(*aL)->info)->masa=((Sala)(*aL)->info)->masa->urm;
        	    	free(aux->info);
        	    	free(aux1);
        	    	((Sala)L->info)->nrMese--;
        	}
        	else{
        	    	while(I->urm!=M1){
        	        	I=I->urm;
         	   	}
         	   	TLista aux1=malloc(sizeof(struct celulag));
         	   	aux1->urm=NULL;
         	   	aux1=I->urm;
         	   	if(I->urm->urm==NULL){
         		       	I->urm=NULL;
        	        	((Sala)(*aL)->info)->nrMese--;
        	    	}
        	    	else{
        	        	I->urm=aux1->urm;
        	    	}
        	    	free(aux1->info);
        	    	free(aux1);
        	    	((Sala)(*aL)->info)->nrMese--;
        	}
    	}
    	if(m==0 && n==1){
        	fprintf(out,"Nu exista suficiente locuri in sala!\n");
    	}
}

TLista InitL(){		//functie pentru initializarea listelor circulare ale jucatorilor
    	TLista aux;
    	aux=(TLista)malloc(sizeof(Celula));
    	if(!aux) return NULL;
    	aux->info=NULL;
    	aux->urm=aux;
    	return aux;
}

int InsLJ(ALista aL, void* ae){		//insereaza jucatorii la mesele corescpunzatoare
    	TLista aux,L,cel;
    	L=*aL;
    	cel=*aL;
    	aux=malloc(sizeof(Celula));
    	aux->info=ae;
    	if(L->urm==L){
        	(*aL)->urm=aux;
        	aux->urm=*aL;
    	}
    	else{
        	L=L->urm;
        	while(L->urm!=cel){
            		L=L->urm;
        	}
        	L->urm=aux;
        	aux->urm=cel;
    	}
    	return 1;
}

int InsLG(ALista aL, void* ae){		//insereaza meselor la lista meselor
    	TLista aux=malloc(sizeof(Celula));
    	TLista L=*aL;    
    	aux->urm = NULL;
    	aux->info = ae;
    	if (!L){
        	*aL = aux;
    	}
    	else{
        	for(; L->urm!=NULL;L=L->urm);
        	L->urm=aux;
    	}
    	return 1;
}

void Distruge(ALista aL){		//distruge listele de jucatori, lista de mese si sala
    	TLista Ma=((Sala)(*aL)->info)->masa;
    	while(Ma!=NULL){
        	TLista Ju=((Masa)Ma->info)->jucatori;
        	Ju=Ju->urm;
        	while(Ju!=NULL){
            		TLista aux=Ju;
            		if(aux->info!=NULL){
                		free(aux->info);
                		free(aux);
            		}
            		else{
                		free(aux);
                		break;
            		}
            		Ju=Ju->urm;
        	}
        	TLista aux=Ma;
        	Ma=Ma->urm;
        	free(aux->info);
		free(aux);
    	}
    	free((*aL)->info);
    	free(*aL);
}

int main (int argc, char*argv[])
{
    	size_t i,j;
    	int N=0,nC=0,nM=0;
    	int rez;
    	int rez1;
    	TLista r=NULL;
    	Masa p = NULL;
    	Jucator q = NULL;
    	FILE *in=fopen(argv[1],"rt"); 	//deschiderea fisierului care contine configuratia meselor si citirea datelor
    	fscanf(in, "%d", &N);
    	for(i=0; i<N; i++){
		p=(Masa)malloc(sizeof(struct masa));
        	char* nMa=malloc(20);
        	fscanf(in,"%s",nMa);
		p->numeMasa=malloc(strlen(nMa)+1);
        	strcpy(p->numeMasa,nMa);
        	fscanf(in,"%d",&p->nrCrtJucatori);
        	fscanf(in,"%d",&p->nrMaxJucatori);
        	nC=nC+p->nrCrtJucatori;
        	nM=nM+p->nrMaxJucatori;
        	p->jucatori=NULL;
        	p->jucatori=InitL();
        	for(j=0;j<p->nrCrtJucatori;j++){
            		q=(Jucator)malloc(sizeof(struct jucator));
            		q->nume=malloc(20);
            		fscanf(in,"%s",q->nume);
            		fscanf(in,"%d",&q->nrMaini);
            		rez1=InsLJ(&p->jucatori,q);
        	}
        	rez=InsLG(&r,p);
        
    	}
    
    	TLista s=malloc(sizeof(Celula));
    	s->info=(Sala)malloc(sizeof(struct sala));
    	((Sala)s->info)->nrMese=N;
    	((Sala)s->info)->nrLocCrt=nC;
    	((Sala)s->info)->nrLocMax=nM;
    	((Sala)s->info)->masa=r;
    	
	
    
    	fclose(in);				//inchiderea fisierului configuratiei
    	FILE *fil=fopen(argv[2],"rt");	//deschiderea fisierului cu instructiuni
    	FILE *out=fopen(argv[3],"wt");	//deschiderea fisierului pentru output

	while(!feof(fil)){			//se citeste pana la finalul fisierului

        	char* com=malloc(20);
        	fscanf(fil,"%s",com);
        	if(strcmp(com,"print")==0){		//executa print
            		PrintM(((Sala)s->info)->masa,out);
        	}
        
        	if(strcmp(com,"noroc")==0){		//executa noroc
            		char* nume_masa=malloc(20);
            		char* nume_jucator=malloc(20);
            		int grad_noroc;
            		fscanf(in,"%s %s %d",nume_masa, nume_jucator, &grad_noroc);
            		Noroc(&((Sala)s->info)->masa,nume_masa,nume_jucator,grad_noroc,out);
			free(nume_masa);
			free(nume_jucator);

        	}
        
        	if(strcmp(com,"ghinion")==0){		//executa ghinion
            		char* nume_masa=malloc(20);
            		char* nume_jucator=malloc(20);
            		int grad_ghinion;
            		fscanf(in,"%s %s %d",nume_masa, nume_jucator, &grad_ghinion);
	    		Ghinion(&s,nume_masa,nume_jucator,grad_ghinion,out);
			free(nume_masa);
			free(nume_jucator);
        	}
        
        	if(strcmp(com,"tura")==0){		//executa tura
            		char* nume_masa=malloc(20);
            		fscanf(in,"%s",nume_masa);
            		Tura(&s, nume_masa,out);
			free(nume_masa);
        	}
        
        	if(strcmp(com,"tura_completa")==0){ 		//executa tura_completa
            		Tura_completa(&s);	    
        	}
        
		if(strcmp(com,"clasament")==0){		//executa clasament
			char* nume_masa=malloc(20);
            		fscanf(in,"%s",nume_masa);
            		Clasament(&s, nume_masa,out);
			free(nume_masa);
        	}
        
        	if(strcmp(com,"inchide")==0){		//executa inchide
            		char* nume_masa=malloc(20);
            		fscanf(in,"%s",nume_masa);
            		Inchide(&s, nume_masa,out);
			free(nume_masa);
        	}
        	free(com);
    	}
    	fclose(fil);		//inchide fisierele
    	fclose(out);
    	Distruge(&s);		//distruge listele

	free(q);
	free(p);
    
    	return 0;
}
