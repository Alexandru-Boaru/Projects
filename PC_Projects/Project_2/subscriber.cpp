#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unordered_map>
#include "helpers.h"
using namespace std;


typedef struct 
{
	char len[4];
	char mes[1600];
}pachet;

//mesaj de avertisment
void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

//mesaj de avertisment
void usage1(char *file){
	fprintf(stderr, "Given name is to long. Maximum number of characters is 10\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	//definire variabile (mai multe detalii in README)
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	char name[10];
	char topic[50];
	int SF;
	int len;
	int rem;
	pachet pack;
	string s="";
	string saux="";
	string slen="";
	if (argc != 4) {
		usage(argv[0]);
	}
	if(strlen(argv[1])>10){
		usage1(argv[0]);
	}
	memset(&name,0,10);
	memcpy(&name,argv[1],strlen(argv[1]));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");
	//se seteaza TCP_NODELAY
	int yes = 1;
	n=setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&yes,sizeof(int));
    DIE(n==-1, "setsockopt");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	//se conecteaza socketul la server
	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	int maxfd = 0;
	fd_set read_set;

	//se pregateste multimea descriptorilor de citire
	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO,&read_set);
	FD_SET(sockfd,&read_set);
	if(sockfd >maxfd){
		maxfd=sockfd;
	}

	//se trimite mesajul new care contine informatiile despre client
	memset(buffer, 0, BUFLEN);
	sprintf(buffer+4,"new %s %s %s",argv[1],argv[2],argv[3]);
	string ac=to_string(strlen(buffer+4));
	while(ac.length()<4){
		ac.insert(0,"0");
	}
	memcpy(buffer,ac.c_str(),4);
	n = send(sockfd, buffer,strlen(buffer)+4,0);
	DIE(n < 0, "send");

	//se intra in bucla de asteptare
	while (1) {
		fd_set tmp = read_set;
		int rc = select(maxfd+1,&tmp,NULL,NULL,NULL);
		if(FD_ISSET(STDIN_FILENO,&tmp)){
		// se citeste de la tastatura
			memset(buffer, 0, BUFLEN);
			memset(topic,0,50);
			fgets(buffer, BUFLEN - 1, stdin);

			//comanda exit are ca efect inchidere subscriber-ului
			if (strncmp(buffer, "exit", 4) == 0) {
				
			}//se verifica comanda subscribe
			else if(strncmp(buffer, "subscribe", 9) == 0) {
				n=sscanf(buffer,"%*s %s %d",topic,&SF);
				
				if(strlen(topic)>50  || SF>1 || SF <0 || n!=2){
					printf("Comanda incorecta %d %d %d\n",(int)strlen(topic),SF,n);
					continue;
				}

			}//se verifica comanda unsubscribe
			else if(strncmp(buffer, "unsubscribe", 11) == 0) {
				n=sscanf(buffer,"%*s %s",topic);
				if(strlen(topic)>50 || n!=1){
					printf("Comanda incorecta %d %d\n",(int)strlen(topic), n);
					continue;
				}
			}
			else{
				printf("Comanda incorecta\n");
				continue;
			}	


			// se trimite mesaj la server
			memset(&pack,0,sizeof(pack));
			string acs=to_string(strlen(buffer));
			while(acs.length()<4){
				acs.insert(0,"0");
			}
			memcpy(pack.len,acs.c_str(),4);
			memcpy(pack.mes,buffer,strlen(buffer));
			n = send(sockfd, &pack, strlen(buffer)+4, 0);
			DIE(n < 0, "send");
		}
		else{
			//se primeste mesaj de la server
			memset(buffer, 0,BUFLEN);
			memset(&pack,0,sizeof(pachet));
			n = recv(sockfd, buffer, sizeof(buffer), 0 );
			DIE(n < 0, "send");
			if(n==0){
				break;
			}
			rem=0;
			len=0;
			//se salveaza continutul buffer-ului in s
			s+=buffer;
			while(!s.empty()){

				if(saux.empty()){
					if(s.size()>=4-slen.size()){//se trece lungimea serializata in slen
						rem=slen.size();
						slen=s.substr(0,4-slen.size());
						s.erase(0,4-rem);
					}
					else{
						slen+=s;
						s.clear();
						break;
					}
				}

				if(s.size()>=stoi(slen)-saux.size()){//se trece payload-ul in saux
					rem=saux.size();
					saux+=s.substr(0,stoi(slen)-saux.size());
					if(strncmp(saux.c_str(),"exit",4)==0){
						printf("Someone is already connected with this id\n");
						n=close(sockfd);
						DIE(n<0,"close");
						return 0;
					}
					if(strncmp(saux.c_str(),"disconnect",10)==0){
						printf("Server disconnected\n");
						n=close(sockfd);
						DIE(n<0,"close");
						return 0;
					}
					printf("%s\n",saux.c_str());
					s.erase(0,stoi(slen)-rem);
					saux.clear();
					slen.clear();
				}
				else{
					saux+=s;
					s.clear();
					break;
				}
			}
		}
	}
	//se inchide socketul
	n=close(sockfd);
	DIE(n<0,"close");
	return 0;
}
