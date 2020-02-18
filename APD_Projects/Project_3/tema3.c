#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mpi.h>

FILE *ptrIn;		//pointer pentru fisierul de input
FILE *ptrOut;		//pointer pentru fisierul de output
int oneColor;		//se verifica daca fisierul e PGM sau PNM
//filtre
char filters[5][10]={"smooth", "blur", "sharpen", "mean", "emboss"};
float filterMatrix[5][3][3] = {{{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0}},
							{{1.0,2.0,1.0},{2.0,4.0,2.0},{1.0,2.0,1.0}},
							{{0.0,-2.0,0.0},{-2.0,11.0,-2.0},{0.0,-2.0,0.0}},
							{{-1.0,-1.0,-1.0},{-1.0,9.0,-1.0},{-1.0,-1.0,-1.0}},
							{{0.0,1.0,0.0},{0.0,0.0,0.0},{0.0,-1.0,0.0}}};
float filterDiv[5] = {9.0,16.0,3.0,1.0,1.0};
int main(int argc, char* argv[]){
	
	int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int checkStatus;	//se verifica daca sunt erori de citire si deschidere fisiere
    unsigned char buffer[100];	//buffer in care se copiaza header-ul 
    unsigned char type[2];		
    unsigned char commentLine[50];
    unsigned char number[10];
    unsigned int width, height;	//dimensiunile pozei
    unsigned int w, h;			//dimensiunile fragmentelor de poza pe care lucreaza un proces
    unsigned char **image, **imageCopy;	//matrice pentru imagine si o copie a ei (prentru PGM)
    unsigned char **r, **rCopy;		//matrice pentru canalul rosu si o copie (prentru PNM)	
    unsigned char **g, **gCopy;		//matrice pentru canalul verde si o copie (prentru PNM)
    unsigned char **b, **bCopy;		//matrice pentru canalul albastru si o copie(prentru PNM)
    int *filtersToApply;		//lista cu filtrele ce urmeaza a fi aplicate
    int nFilters;				//numarul filtrelor ce urmeaza a fi aplicate
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    //procesul 0 se ocupa cu citirea datelor si cu distribuirea de fragmente catre celelalte procese
    if(rank==0){
    	checkStatus = 1;

    	//se verifica argumentele primite
    	if(argc < 3){
			printf("Not enough arguments\n");
			printf("Format: ./tema3 image_in image_out <filters>\n");
			checkStatus = 0;
		}
		if(checkStatus==1){
			ptrIn = fopen(argv[1],"rb");
			ptrOut = fopen(argv[2],"wb");
			if(ptrIn == NULL || ptrOut == NULL){
				printf("file error\n");
				checkStatus = 0;
			}
			if(checkStatus==1){

				//daca totul e in regula se incepe citirea header-ului

				unsigned char ch;
				int i=0;
				int j=0;
				int part = 0;
				int start, end;
				memset(buffer, 0, sizeof(char));
				while(1){
					ch = fgetc(ptrIn);
					if(part!=1)
						buffer[i++] = ch;
					
					if(ch=='\n')
						part++;
					if(part==2 && ch =='\n')
						start=i;
					if(part==4)
						break;
				}
				len = i;

				//se copiaza dimensiunile imaginii
				sscanf(buffer+start, "%d %d", &width, &height);
				buffer[i++]='\0';

				//se pregateste lista filtrelor
				filtersToApply = calloc(argc-3, sizeof(int));
				char c[10];
				for(int j = 3; j < argc; j++){
					for(int k = 0; k < 5; k++){
						strcpy(c,argv[j]);
						if(strcmp(c, filters[k])==0){
							filtersToApply[j-3]=k+1;
						}
					}
				}
				nFilters = argc - 3;


				if(buffer[1]=='5'){
					//alocare matrice pentru PGM
					oneColor=1;
					image = calloc(height+2, sizeof(char*));
					for(i = 0; i < height+2; i++){
						image[i] = calloc(width+2, sizeof(char));
					}
				}
				else{
					//alocare matrice pentru PNM
					oneColor=0;
					r = calloc(height+2, sizeof(char*));
					g = calloc(height+2, sizeof(char*));
					b = calloc(height+2, sizeof(char*));
					for(i = 0; i < height+2; i++){
						r[i] = calloc(width+2, sizeof(char));
						g[i] = calloc(width+2, sizeof(char));
						b[i] = calloc(width+2, sizeof(char));
					}
				}


				if(oneColor==1){
					//citire date din PGM
					for(i = 1; i < height+1; i++){
						fread(image[i]+1, 1, width, ptrIn);
					}
				}
				else{
					//citire date din PNM
					for(i = 1; i < height+1; i++){
						for(int j = 1; j<width+1; j++){
							r[i][j] = fgetc(ptrIn);
							g[i][j] = fgetc(ptrIn);
							b[i][j] = fgetc(ptrIn);
						}
					}
				}


				int chunk = height / numtasks;
				int bonus = height - chunk * numtasks;
				int temp;
				w = width;

				//se trimit un numar egal de linii de date la toate procesele 
				//(mai putin ultimul proces care poate avea cateva linii in minus
				for(start = 1, end = start + chunk, i = 0; start < height, i < numtasks; start = end, end = start + chunk, i++){
					if(bonus){
						end++;
						bonus--;
					}
					if(i == 0){
						temp = end - start;
						continue;
					}
					h = end - start;

					//se trimit dimensiunile fragmentelor catre procese
					MPI_Send(&w, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
					MPI_Send(&h, 1, MPI_INT, i, 99, MPI_COMM_WORLD);

					//se trimit liniile de date
					for(int j = start-1; j < end+1; j++){
						if(oneColor==1)
							MPI_Send(image[j], width+2, MPI_CHAR, i, 99, MPI_COMM_WORLD);
						else{
							MPI_Send(r[j], width+2, MPI_CHAR, i, 99, MPI_COMM_WORLD);
							MPI_Send(g[j], width+2, MPI_CHAR, i, 99, MPI_COMM_WORLD);
							MPI_Send(b[j], width+2, MPI_CHAR, i, 99, MPI_COMM_WORLD);
						}
					}
				}
				h = temp;
			}
			
		}
			
	}
	//se transmite checkStatus catre toate procesele pentru a verifica daca se mai fac urmatoarele operatii
	MPI_Bcast(&checkStatus, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Bcast(&oneColor, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//se calculeaza matricile filtrelor
	for(int ii = 0; ii < 5; ii++){
		for(int jj = 0; jj < 3; jj++){
			for(int zz = 0; zz < 3; zz++){
				filterMatrix[ii][jj][zz] = (float) filterMatrix[ii][jj][zz] / filterDiv[ii];
			}
		}
	}


	if(checkStatus == 1 && rank != 0){

		//se primesc dimensiunile fragmentelor
		MPI_Recv(&w, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&h, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//se aloca matricile
		if(oneColor==1){
			image = calloc(h+2, sizeof(char*));
			for(int i = 0; i < h+2; i++){
				image[i] = calloc(w+2, sizeof(char));
			}
		}
		else{
			r = calloc(h+2, sizeof(char*));
			g = calloc(h+2, sizeof(char*));
			b = calloc(h+2, sizeof(char*));
			for(int i = 0; i < h+2; i++){
				r[i] = calloc(w+2, sizeof(char));
				g[i] = calloc(w+2, sizeof(char));
				b[i] = calloc(w+2, sizeof(char));
			}
		}
		int i;

		//se primesc datele
		for(i = 0; i < h + 2; i++){
			if(oneColor==1)
				MPI_Recv(image[i], w+2, MPI_CHAR, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			else{
				MPI_Recv(r[i], w+2, MPI_CHAR, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(g[i], w+2, MPI_CHAR, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(b[i], w+2, MPI_CHAR, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}		
	}

	//se transmite lista filtrelor la toate procesele
	if(checkStatus == 1){
		MPI_Bcast(&nFilters, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if(rank!=0){
			filtersToApply = calloc(nFilters, sizeof(int));
		}
		MPI_Bcast(filtersToApply, nFilters, MPI_INT, 0, MPI_COMM_WORLD);
	}

	if(checkStatus == 1){

		float currentK;
		float currentFilter[3][3];

		//se creaza o copie a matricii/matricelor
		if(oneColor==1){
			imageCopy = calloc(h+2, sizeof(char*));
			for(int i = 0; i < h+2; i++){
				imageCopy[i] = calloc(w+2, sizeof(char));
			}
		}
		else{
			rCopy = calloc(h+2, sizeof(char*));
			gCopy = calloc(h+2, sizeof(char*));
			bCopy = calloc(h+2, sizeof(char*));
			for(int i = 0; i < h+2; i++){
				rCopy[i] = calloc(w+2, sizeof(char));
				gCopy[i] = calloc(w+2, sizeof(char));
				bCopy[i] = calloc(w+2, sizeof(char));
			}
		}

		//se aplica fiecare filtru in parte
		for(int job = 0; job < nFilters; job++){
			if(filtersToApply[job]==0){
				printf("WRONG FILTER\n");
				continue;
			}
			//se seteaza matricea necesara filtrului
			switch(filtersToApply[job]){
				case 1:
					memcpy(currentFilter, filterMatrix[0], sizeof(currentFilter));
					break;
				case 2:
					memcpy(currentFilter, filterMatrix[1], sizeof(currentFilter));
					break;
				case 3:
					memcpy(currentFilter, filterMatrix[2], sizeof(currentFilter));
					break;
				case 4:
					memcpy(currentFilter, filterMatrix[3], sizeof(currentFilter));
					break;
				case 5:
					memcpy(currentFilter, filterMatrix[4], sizeof(currentFilter));
					break;
			}
			
			float temp;
			float temp1,temp2,temp3;

			//se aplica filtrul pe fiecare element in parte
			//se face cast la float cat timp se aplica filtrele
			for(int i = 1; i < h+1; i++){
				for(int j = 1; j< w+1; j++){
					temp = 0.0;
					temp1 = 0.0;
					temp2 = 0.0;
					temp3 = 0.0;
					for(int ii=-1; ii<2; ii++){
						for(int jj=-1; jj<2; jj++){
							if(oneColor==1)
								temp += (float)image[i+ii][j+jj]*currentFilter[1-ii][1-jj];
							else{
								temp1 += (float)r[i+ii][j+jj]*currentFilter[1-ii][1-jj];
								temp2 += (float)g[i+ii][j+jj]*currentFilter[1-ii][1-jj];
								temp3 += (float)b[i+ii][j+jj]*currentFilter[1-ii][1-jj];
							}
						}
					}
					//se clampeaza valorile
					if(temp>255.0){
						temp = 255.0;
					}
					if(temp	< 0.0){
						temp = 0.0;
					}
					if(temp1>255.0){
						temp1 = 255.0;
					}
					if(temp1< 0.0){
						temp1 = 0.0;
					}
					if(temp2>255.0){
						temp2 = 255.0;
					}
					if(temp2< 0.0){
						temp2 = 0.0;
					}
					if(temp3>255.0){
						temp3 = 255.0;
					}
					if(temp3< 0.0){
						temp3 = 0.0;
					}

					//se casteaza inapoi la unsigned char
					if(oneColor==1)
						imageCopy[i][j] = (unsigned char) temp;
					else{
						rCopy[i][j] = (unsigned char) temp1;
						gCopy[i][j] = (unsigned char) temp2;
						bCopy[i][j] = (unsigned char) temp3;
					}
				}
			}

			//se actualizeaza datele intermediare
			for(int i = 1; i < h + 1; i++){
				if(oneColor==1)
					memcpy(image[i], imageCopy[i], w+2);
				else{
					memcpy(r[i], rCopy[i], w+2);
					memcpy(g[i], gCopy[i], w+2);
					memcpy(b[i], bCopy[i], w+2);
				}
			}

			//se transmite o linie cu rol de bordura superioara catre procesul care se ocupa cu fragmentul de mai sus
			if(rank > 0){
				if(oneColor==1)
					MPI_Send(image[1], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD);
				else{
					MPI_Send(r[1], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD);
					MPI_Send(g[1], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD);
					MPI_Send(b[1], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD);
				}
			}
			//se transmite o linie cu rol de bordura inferioara catre procesul care se ocupa cu fragmentul de mai jos
			if(rank < numtasks-1){
				if(oneColor==1)
					MPI_Send(image[h], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD);
				else{
					MPI_Send(r[h], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD);
					MPI_Send(g[h], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD);
					MPI_Send(b[h], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD);
				}
			}

			//se primeste o bordura superioara actualizata de la procesul care se ocupa cu fragmentul de mai sus
			if(rank > 0){
				if(oneColor==1)
					MPI_Recv(image[0], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				else{
					MPI_Recv(r[0], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(g[0], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(b[0], w+2, MPI_CHAR, rank-1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
			//se primeste o bordura inferioara actualizata de la procesul care se ocupa cu fragmentul de mai jos
			if(rank < numtasks-1){
				if(oneColor==1)
					MPI_Recv(image[h+1], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				else{
					MPI_Recv(r[h+1], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(g[h+1], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(b[h+1], w+2, MPI_CHAR, rank+1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}

		//se elibereaza memoria alocate
		for(int i = 0; i < h+2; i++){
			if(oneColor==1)
				free(imageCopy[i]);
			else{
				free(rCopy[i]);
				free(gCopy[i]);
				free(bCopy[i]);
			}
		}
		if(oneColor==1)
			free(imageCopy);
		else{
			free(rCopy);
			free(gCopy);
			free(bCopy);
		}
	}

	//se trimit fragmentele catre procesul 0
	if(checkStatus == 1 && rank != 0){
		int i;
		for(i = 1; i < h + 1; i++){
			if(oneColor==1)
				MPI_Send(image[i], w+2, MPI_CHAR, 0, 22, MPI_COMM_WORLD);
			else{
				MPI_Send(r[i], w+2, MPI_CHAR, 0, 22, MPI_COMM_WORLD);
				MPI_Send(g[i], w+2, MPI_CHAR, 0, 22, MPI_COMM_WORLD);
				MPI_Send(b[i], w+2, MPI_CHAR, 0, 22, MPI_COMM_WORLD);
			}
		}
	}

	//procesul 0 primeste fragmentele cu datele finale
	if(checkStatus == 1 && rank == 0){
		int chunk = height / numtasks;
		int bonus = height - chunk * numtasks;
		int i, start1, end1;
		for(start1 = 1, end1 = start1 + chunk, i = 0; start1 < height, i < numtasks; start1 = end1, end1 = start1 + chunk, i++){
			if(bonus){
				end1++;
				bonus--;
			}
			if(i == 0)
				continue;

			for(int j = start1; j < end1; j++){
				if(oneColor==1)
					MPI_Recv(image[j], width+2, MPI_CHAR, i, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				else{
					MPI_Recv(r[j], width+2, MPI_CHAR, i, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(g[j], width+2, MPI_CHAR, i, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(b[j], width+2, MPI_CHAR, i, 22, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}


		//se scrie header-ul in output
		fwrite(buffer, 1, len, ptrOut);

		//se scriu datele finale in output
		for(int i = 1; i < height + 1; i++){
			if(oneColor==1)
				fwrite(image[i]+1, 1, width, ptrOut);
			else{
				for(int j = 1; j < width + 1; j++){
					fputc(r[i][j], ptrOut);
					fputc(g[i][j], ptrOut);
					fputc(b[i][j], ptrOut);
				}
			}
		}

		//se elibereaza memoria

		for(int i = 0; i < height+2; i++){
			if(oneColor==1)
				free(image[i]);
			else{
				free(r[i]);
				free(g[i]);
				free(b[i]);
			}
		}
		if(oneColor==1)
			free(image);
		else{
			free(r);
			free(g);
			free(b);
		}
		free(filtersToApply);

		//se inchid fisierele
		fclose(ptrIn);
		fclose(ptrOut);
	}

	//se elibereaza memoria din celelalte procese
	if(checkStatus == 1 && rank != 0){
		for(int i = 0; i < h+2; i++){
			if(oneColor==1)
				free(image[i]);
			else{
				free(r[i]);
				free(g[i]);
				free(b[i]);
			}
		}
		if(oneColor==1)
			free(image);
		else{
			free(r);
			free(g);
			free(b);
		}
		free(filtersToApply);
	}

	MPI_Finalize();
}