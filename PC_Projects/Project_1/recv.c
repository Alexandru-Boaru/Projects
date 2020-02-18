#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

//structura folosita pentru stocarea informatiei unui pachet
typedef struct{
	short secv;
	char xor_char;
	char info[MSGSIZE-sizeof(short)-sizeof(char)];
}cadru;

//functie care calculeaza xor peste informatie
char xor_check(char* payload, int secv, size_t size){
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

//buffer-ul pentru mesaje
msg window[7200];

int main(){
  msg r,t;
  cadru c;
  init(HOST,PORT);
  char *buff;
while(1){ //se asteapta numele fisierului de output si trimite confirmare
  if (recv_message(&r)<0){
    perror("Receive message");
    return -1;
  }

    memcpy(&c,r.payload,sizeof(r.payload));

    if(c.xor_char == xor_check(c.info,c.secv,strlen(c.info))){
        sprintf(t.payload,"ACK %d",c.secv);
        t.len = strlen(t.payload+1);
        send_message(&t);
	break;
    }
}
  buff = strtok(c.info," ");
  int fd = open(buff,O_WRONLY | O_CREAT | O_TRUNC, 0644); //se deschide fisierul de output
  if(fd<0){
	perror("deschidere");
	exit(-1);
  }
  
  buff = strtok(NULL," ");
  int w = atol(buff);
  buff = strtok(NULL," ");
  int packnum = atol(buff);
  memset(&window,0,sizeof(window));
  int begin_w=0;
  int end_w=w-1;

    while(1){ //se asteapta pachetele
        memset(t.payload,0,sizeof(t.payload));
        memset(c.info,0,sizeof(c.info));
        if(recv_message(&r)<0){
        }
        else {
            memcpy(&c,r.payload,sizeof(r.payload));
            if(c.xor_char == xor_check(c.info,c.secv,r.len)){ //se verifica daca pachetul este corupt

            	memcpy(&window[getSecv(r)-1],&r,sizeof(r)); //se pune mesajul in buffer
            	if(end_w<getSecv(r)-1){
            	    end_w=getSecv(r)-1;
            	}
            	while(getSecv(window[begin_w])!=0){	//se citeste din buffer atata timp cat are puse in ordine pachetele primite
            	    memcpy(&t,&window[begin_w],sizeof(t));//se opreste cand gaseste un loc gol in buffer
            	    memcpy(&c,t.payload,sizeof(c));
           	    write(fd,c.info,t.len);
           	    if(getSecv(window[begin_w])==packnum && isEmpty(window,packnum) && begin_w==end_w){
           	        break;
           	    }
           	    begin_w++;
           	    if(end_w+1!=packnum){
           	         end_w++;
                    }
               }
               sprintf(t.payload,"ACK %d",getSecv(r)); //se pregateste si se trimite ACK-ul
               t.len = strlen(t.payload+1);
               send_message(&t);
            }
        }
        if(begin_w>end_w){	//daca s-a primit ultimul pachet, atunci se trimite un mesaj cu lungime zero si se iese din bucla
	    t.len=0;
	    send_message(&t);
            break;
        }
    }
    close(fd); // se inchide fisierul de intrare
    return 0;
}
