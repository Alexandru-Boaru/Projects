#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

//structura folosita pentru stocarea informatiei unui pachet
typedef struct{
	short secv;
	char xor_char;
	char info[MSGSIZE-sizeof(short)-sizeof(char)];
}cadru;

//functie care calculeaza xor peste informatie
char xor_check(char* payload, short secv, size_t size){
	int result = payload[0];
	for(int i=1; i<(int)size; i++){
		result ^= payload[i];
	}
	while(secv){
		result ^=(secv & 1);
		secv>>=1;
	}
	return result;
}

//functie care returneaza numarul unui pachet
int getSecv(msg t){
    cadru c;
    memcpy(&c,t.payload,sizeof(t.payload));
    return c.secv;
}

//functie folosita pentru ciclarea prin vector
int cycle(int num,int w){
    if(num>=w){
        return num -= w;
    }
    else{
        return num;
    }
}

//functie care verifica daca buffer-ul de msg-uri este gol
int isEmpty(msg* window,int packnum){
    for(int i=0; i<packnum;i++){
        if(getSecv(window[i])!=0){
            return 0;
        }
    }
    return 1;
}

//functie care verifica daca un vector dat ca parametru este gol
int isEmptyVec(short vec[],int num){
    for(int i=0; i<num;i++){
        if(vec[i]!=0){
            return 0;
        }
    }
    return 1;
}

//functie care verifica daca un numar dat ca parametru se regaseste intr-un vector de dimensiune num
int contains(short vec[], int num, short search){
	for(int i=0; i<num; i++){
		if(vec[i]==search){
			return 1;
		}
	}
	return 0;
}


//buffer-ul pentru mesaje
msg window[7200];

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t,r;
  cadru c;
  printf("%d",argc);
  char str1[20]="recv_";
  char str2[10];
  strcpy(str2,argv[1]);
  strcat(str1,str2);
  int secv=1;  
  int speed = atol(argv[2]);
  int delay = atol(argv[3]);
  int w = (speed*delay*1000)/(sizeof(msg)*8); //se calculeaza dimensiunea ferestrei
  int fd = open(argv[1],O_RDONLY); //se deschide fisierul de intrare

  int fsize = lseek(fd,0,SEEK_END); //se cauta dimensiunea fisierului

  lseek(fd,0,SEEK_SET);


  int packnum=fsize/(MSGSIZE-sizeof(short)-sizeof(char))+1; //se calculeaza numarul de pachete ce se vor genera

  w=w<packnum?w:packnum;

  //se pregateste si trimite pachetul cu nume
  sprintf(c.info,"%s %d %d %d",str1,w,packnum,fsize);
  c.secv=0;
  c.xor_char=xor_check(c.info,c.secv,strlen(c.info));
  memcpy(t.payload,&c,sizeof(c));
  t.len = strlen(t.payload)+1;
  send_message(&t);
  //se astepta confirmare
  while(1){
      if (recv_message_timeout(&r,delay)<0){
          perror("receive error");
          send_message(&t);
      }
      else {
          char temp[10];
          int secv_check;
          sscanf(r.payload,"%s %d",temp,&secv_check);
          
          if(strcmp(temp,"ACK")==0){
              break;
          }
      }
  }

    memset(&window,0,sizeof(window));
    
    int begin_w=0;
    int end_w=w-1;
   //se citeste din fisier si se salveza pachetele in buffer
    for(int i=0; i<packnum; i++){
        memset(t.payload,0,sizeof(t.payload));
        memset(c.info,0,sizeof(c.info));
        
        int rc=read(fd,c.info,sizeof(c.info));
        c.secv=secv;
        secv++;
        c.xor_char=xor_check(c.info,c.secv,sizeof(c.info));
        t.len=rc;
        memcpy(t.payload,&c,sizeof(c));
        memcpy(&window[i],&t,sizeof(t));    
    }
    
    //se creaza vectorul cu numerele de pachet pe care se asteapta se le primeasca
    short EList[w];
    int expecting=0;
    //se trimit primele w pachete
    for(int i=begin_w; i<end_w+1; i++){
	EList[expecting]=getSecv(window[i]);
	expecting=cycle(expecting+1,w);

        send_message(&window[i]);
	
    }

    char temp[10];
    int secv_check;
    int expecting_test=0;
    //se asteapta ACK-urile
    while(1){
        if (recv_message_timeout(&r,delay+5)<0){
	    //in caz timeout se retrimite cel mai vechi pachet si expecting_test se muta pe urmatoarea pozitie
	    send_message(&window[EList[expecting_test]-1]);

	    while(EList[expecting_test]==0){
	    	expecting_test=cycle(expecting_test+1,w);
	    }
	    continue;
            
        }
        else{
	    if(r.len==0){ //mesaj de lungime 0 de la recv -> se iese din bucla
		break;
	    }
            
            sscanf(r.payload,"%s %d",temp,&secv_check);
            
	    if(contains(EList,w,secv_check)==0){ //daca ACK-ul primit nu e in EList, atunci se ignora
		continue;
	    }

	    while(EList[expecting_test]!=secv_check){//se cauta numarul primit din ACK prin EList si retrimit pachetele necomfirmate
		if(EList[expecting_test]!=0){
                	send_message(&window[EList[expecting_test]-1]);
			
		}
		expecting_test=cycle(expecting_test+1,w);
            }
            memset(&window[secv_check-1],0,sizeof(msg)); //se sterge pachetul confirmat din buffer

	    if(secv_check==packnum && isEmpty(window,packnum)){
                break;
            }
            if(secv_check>=getSecv(window[begin_w])){
                
            	while(1){	//actualizare begin_w
                	if(getSecv(window[begin_w]) == 0 && begin_w<=end_w){
                	    begin_w++;
                	}
                	else{
                	    break;
                	}
            	}        
            }
	    if(begin_w>=end_w){
		break;
	    }
	    
	    
	    if(end_w+1==packnum){	//actualizare end_w si expecting_test
		EList[expecting_test]=0;
		while(EList[expecting_test]==0){
			expecting_test=cycle(expecting_test+1,w);
			if(isEmptyVec(EList,w)){
				break;
			}
		}
                continue;
            }
	    else{
            	end_w++;
	    	EList[expecting_test]=getSecv(window[end_w]);
	    	expecting_test=cycle(expecting_test+1,w);
	    }
        }
        send_message(&window[end_w]); // se trimite pachet nou

    }

    
    close(fd); // se inchide fisierul de intrare
    return 0;
}
