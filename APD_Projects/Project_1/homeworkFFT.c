//BOARU ALEXANDRU 331CB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>
#include <string.h>

typedef double complex cplx;

//declararea unei structuri ce urmeaza a fi folosita pentru stocarea argumentelor ce trebuie
//trimise functiei fastFourierThread
typedef struct{
	int id;		//id-ul threadului

	int order;	//codul primit ca model foloseste doi vectori in calculul valorilor finale;
				//am decis sa folosesc o implementare asemanatoare, dar in loc de 2 vectori 
				//folosesc o matrice cu 2 linii (linii care functioneaza pe post de vectori);
				//folosind order pot sa tin cont mai usor de care vector trebuie sa salveze datele 
				//temporare la orice pas in recurenta
	
	int begin;	//folosit pentru a tine cont de indicele de la care pornesti iterarea in vectori

	int step;	//folosit in acelasi mod ca si in codul primit ca model
}fftInfo;

//declararea variabilelor globale
int P;				//nr de threaduri = parametrul numThreads
int N;				//nr de elemente
int err;			//folosit pentru detectarea erorilor
FILE *ptrIn;		//pointer pentru fisierul de input
FILE *ptrOut;		//pointer pentru fisierul de output
char pathIn[500];	//numele fisierului de input = inputValues.txt
char pathOut[500];	//numele fisierului de output = outputValues.txt
double* values;		//vector in care se salveaza valorile de input
cplx* buf[2];		//matrice in care se salveaza valorile intermediare si cele finale

//functie care se ocupa cu citirea parametrilor programului
void getArgs(int argc, char **argv)
{
	if(argc < 4) {
		printf("Not enough paramters: ./program inputValues.txt outputValues.txt numThreads\n");
		exit(1);
	}
	strcpy(pathIn,(char*)argv[1]);
	strcpy(pathOut,(char*)argv[2]);
	P = atoi(argv[3]);
}

//functie apelata de thread pentru a continua recurenta
void fastFourier(int order, int begin, int step){
	if(step < N){
		fastFourier(1-order, begin, 2*step);
		fastFourier(1-order, begin + step, 2*step);

		for(int i = 0; i < N; i += 2*step){
			cplx temp = cexp(-I * M_PI * i/ N) * buf[1-order][begin+i+step];
			
			buf[order][begin + i/2] = buf[1-order][begin+i] + temp;
			buf[order][begin + (i+N)/2] = buf[1-order][begin+i] - temp;
		}
	}
}

//functia creata de thread
void* fastFourierThread(void *args){
	fftInfo info = *(fftInfo*) args;
	int begin = info.begin;
	int order = info.order;
	int step = info.step;

	if(step < N){
		fastFourier(1-order, begin, 2*step);
		fastFourier(1-order, begin + step, 2*step);

		for(int i = 0; i < N; i += 2*step){
			cplx temp = cexp(-I * M_PI * i/ N) * buf[1-order][begin+i+step];
			
			buf[order][begin + i/2] = buf[1-order][begin+i] + temp;
			buf[order][begin + (i+N)/2] = buf[1-order][begin+i] - temp;
		}
	}

	return NULL;
}

//functie care se ocupa cu crearea threadurilor si cu recursion unrolling in functie de P
void beforeFastFourier(){
	int i;
	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	//pregatirea argumentelor ce sunt trimise functiilor threadurilor nou create
	fftInfo info[P];
	
	for(i = 0; i < P; i++){
		info[i].id = thread_id[i];
		info[i].step = P;
	}
	//valorile sunt puse in mod particular in functie de nivelul de recursion unrolling (ales in functie de P)
	switch(P){
		case 1:
			info[0].begin = 0;

			info[0].order = 0;
			break;
		case 2:
			info[0].begin = 0;
			info[1].begin = 1;

			info[0].order = 1;
			info[1].order = 1;
			break;
		case 4:
			info[0].begin = 0;
			info[1].begin = 2;
			info[2].begin = 1;
			info[3].begin = 3;

			info[0].order = 0;
			info[1].order = 0;
			info[2].order = 0;
			info[3].order = 0;
			break;
	}

	//crearea threadurilor
	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, fastFourierThread, &(info[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	//in cazul in care sunt 4 threaduri (recursion unrolling pana la al doilea nivel), atunci mai sunt de facut
	//cateva calcule final
	if(P==4 && N>=4){
		for(int i = 0; i < N; i += 4){
			cplx temp = cexp(-I * M_PI * i/ N) * buf[0][i+2];
			buf[1][i/2] = buf[0][i] + temp;
			buf[1][(i+N)/2] = buf[0][i] - temp;

			temp = cexp(-I * M_PI * i/ N) * buf[0][1 + i+2];
			buf[1][1+i/2] = buf[0][1+i] + temp;
			buf[1][1+(i+N)/2] = buf[0][1+i] - temp;
		}
	}
	//in cazul in care sunt 2/4 threaduri (recursion unrolling pana la primul/al doilea nivel), atunci mai sunt 
	//de facut cateva calcule final
	if(P>=2 && N>=2){
		for(int i = 0; i < N; i += 2){
			cplx temp = cexp(-I * M_PI * i/ N) * buf[1][i+1];
			buf[0][i/2] = buf[1][i] + temp;
			buf[0][(i+N)/2] = buf[1][i] - temp;
		}
	}
}

//functie de scriere a valorilor finale sub forma de perechi (numar real, numar imaginar) in fisierul de output
void writeToFile(){
	fprintf(ptrOut,"%d\n",N);
	for(int i = 0; i < N; i++){
		fprintf(ptrOut,"%lf %lf\n",creal(buf[0][i]),cimag(buf[0][i]));
	}
}



void init(){
	ptrIn = fopen(pathIn,"r");
	ptrOut = fopen(pathOut,"w");
	if(ptrIn == NULL || ptrOut == NULL){
		printf("file error\n");
		exit(1);
	}
	//citire N
	err = fscanf(ptrIn, "%d", &N);
	if(err < 0){
		printf("input error\n");
		exit(1);
	}
	//alocarea dinamica a datelor
	values = malloc(sizeof(double)*N);
	buf[0] = malloc(sizeof(cplx)*N);
	buf[1] = malloc(sizeof(cplx)*N);

	if(values == NULL || buf == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	//citirea valorilor din fisier
	for(int i =0; i < N; i++){
		err = fscanf(ptrIn, "%lf", &values[i]);
		if(err < 0){
			printf("input error\n");
			exit(1);
		}
		buf[0][i] = values[i];
		buf[1][i] = values[i];
 	}
}




int main(int argc, char * argv[]){
	//citire argumente
	getArgs(argc,argv);

	//citire valori si alocare vectori
	init();

	//crearea threadurilor
	beforeFastFourier();

	//scriere in output
	writeToFile();

	//dezalocare vectori si inchidere fisiere
	free(values);
	free(buf[0]);
	free(buf[1]);
	fclose(ptrIn);
	fclose(ptrOut);

	return 0;
}