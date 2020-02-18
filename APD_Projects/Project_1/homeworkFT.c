//BOARU ALEXANDRU 331CB

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>
#include <string.h>

typedef double complex cplx;

//declararea variabilelor globale
int P;				//nr de threaduri = parametrul numThreads
int N;				//nr de elemente
int err;			//folosit pentru detectarea erorilor
FILE *ptrIn;		//pointer pentru fisierul de input
FILE *ptrOut;		//pointer pentru fisierul de output
char pathIn[500];	//numele fisierului de input = inputValues.txt
char pathOut[500];	//numele fisierului de output = outputValues.txt
double* values;		//vector in care se salveaza valorile de input
cplx* buf;			//vector in care se salveaza valorile finale

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

//functie executata de threaduri
//realizeaza calculul valorilor finale folosind formula transformarilor Fourier discrete
void* discreteFourier(void *args){

	int thread_id = *(int*)args;

	//am decis sa paralelizez bucla for exterioara; fiecare thread executa un numar de iteratii egal cu N/P
	//ultimul thread poate sa aiba mai putine iteratii daca N%P == 0
	int frag = N/P;
	int begin = thread_id*frag;
	int end = (thread_id+1)*frag;
	if(end > N){
		end = N;
	}
	int n,k;
	for(k = begin; k < end; k++){
		for(n = 0; n < N; n++){
			buf[k] += values[n] * cexp(-I * 2 * M_PI * k * n /N);
		}
	}

	return NULL;
}

//functie care se ocupa cu alocarea vectorilor si cu citirea din fisierul de input
void init(){
	ptrIn = fopen(pathIn,"r");	//deschidere fisier input
	ptrOut = fopen(pathOut,"w");//deschidere fisier output
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
	//alocarea dinamica a vectorilor
	values = malloc(sizeof(double)*N);
	buf = malloc(sizeof(cplx)*N);

	//citirea valorilor din fisier
	for(int i =0; i < N; i++){
		err = fscanf(ptrIn, "%lf", &values[i]);
		if(err < 0){
			printf("input error\n");
			exit(1);
		}
 	}
}

//functie de scriere a valorilor finale sub forma de perechi (numar real, numar imaginar) in fisierul de output
void writeToFile(){
	fprintf(ptrOut,"%d\n",N);
	for(int i = 0; i < N; i++){
		fprintf(ptrOut,"%lf %lf\n",creal(buf[i]),cimag(buf[i]));
	}
}

//functie main in care sunt create threadurile si se apeleaza restul functiilor
int main(int argc, char * argv[]) {
	int i;
	//citire argumente
	getArgs(argc,argv);
	//citire valori si alocare vectori
	init();

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	//creare threaduri
	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, discreteFourier, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}
	//scriere in output
	writeToFile();

	//dezalocare vectori si inchidere fisiere
	free(values);
	free(buf);
	fclose(ptrIn);
	fclose(ptrOut);

	return 0;
}
