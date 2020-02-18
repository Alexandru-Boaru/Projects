#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <list>
#include "helpers.h"
using namespace std;

typedef struct 
{
	string id;
	string ip;
	int port;
}subscriber;

typedef struct 
{
	char len[4];
	char mes[1600];
}pachet;

typedef struct{
	pachet pack;
	unordered_set<string> cli;
}stored;

typedef struct{
	char com[100]; 
}comanda;


typedef struct{
    char topic[50];
    char tip;
    char payload[1500]; 
}message;


//mesaj de avertisment
void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

//functie care trimite feedback catre clienti
void feedback(string topic, int i, int t){
	string mesaj="";
	switch(t){
		case 0:
			mesaj+="subscribed to ";
			break;
		case 1:
			mesaj+="unsubscribed from ";
			break;
		case 2:
			mesaj+="not subscribed to ";
			break;
		case 3:
			mesaj+="this topic doesn't exist ";
			break;
		case 4:
			mesaj+="exit";
		case 5:
			mesaj+="disconnect";
	}
	if(t!=4 && t!=5){
		mesaj+=topic;
	}
	string acs=to_string(mesaj.size());
	while(acs.length()<4){
		acs.insert(0,"0");
	}
	mesaj.insert(0,acs);
	int sr = send(i,mesaj.c_str(),mesaj.size(),0);
	DIE(sr<0,"send error");
}

int main(int argc, char *argv[])
{
	//declarare variabile
	int sockTCP, newsockfd, portno;
	int sockUDP;
	char buffer[BUFLEN];
	char name[10];
	char ip[20];
	char port[10];
	char topic[50];
	int SF;
	int rem=0;
	int len=0;
	string s="";
	string saux="";
	string slen="";
	pachet pack;
	stored stor;
	struct sockaddr_in serv_addr, cli_addr;
	message mes;
	int n, i, j, ret;
	socklen_t clilen;
	int st=0;
	fd_set read_fds;	// multimea de citire folosita in select()
	fd_set tmp_fds;		// multime folosita temporar
	int fdmax;			// valoare maxima fd din multimea read_fds
	int sid=0;
	unordered_set<string> clientACT;
	map<int,stored> storage;
    unordered_map<int, subscriber> clientTCP_map;
    unordered_map<string,unordered_map<string,int> > topic_map;
    vector<comanda> comenzi;
	if (argc < 2) {
		usage(argv[0]);
	}

	// se goleste multimea de descriptori de citire (read_fds) si multimea temporara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockTCP = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockTCP < 0, "socket");

	int yes = 1;
	n=setsockopt(sockTCP,IPPROTO_TCP,TCP_NODELAY,&yes,sizeof(int));
    DIE(n==-1, "setsockopt");

	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	sockUDP = socket(AF_INET, SOCK_DGRAM,0);
	DIE(sockUDP < 0, "socket");

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	//se face bind la socketii de tcp si de udp
	ret = ::bind(sockTCP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	ret = ::bind(sockUDP, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	ret = listen(sockTCP, MAX_CLIENTS);
	DIE(ret < 0, "listen");

	// se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockTCP, &read_fds);
	FD_SET(sockUDP, &read_fds);
	FD_SET(STDIN_FILENO,&read_fds);
	fdmax = (sockTCP>sockUDP)?sockTCP:sockUDP;

	while (1) {
		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		if(FD_ISSET(STDIN_FILENO,&tmp_fds)){//se citeste de la tastatura
			memset(buffer, 0, BUFLEN);
			
			fgets(buffer, BUFLEN - 1, stdin);

			if (strncmp(buffer, "exit", 4) == 0) {
				
				for(j=0;j<=fdmax;j++){
					if(j>4 && j!=sockTCP && j!=sockUDP && j!=STDIN_FILENO){
						if(clientTCP_map[j].id!=""){
							feedback("",j,5);
						}
						
					}
				}
				break;
			}
			else{
				printf("Comanda incorecta\n");
				continue;
			}
		}	
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockTCP) {//se primeste mesaj pe socketul TCP
					memset(&serv_addr, 0, sizeof(serv_addr));
					memset(&cli_addr, 0, sizeof(cli_addr));
					// a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
					// pe care serverul o accepta
					clilen = sizeof(cli_addr);
					newsockfd = accept(sockTCP, (struct sockaddr *) &cli_addr, &clilen);
					DIE(newsockfd < 0, "accept");

					// se adauga noul socket intors de accept() la multimea descriptorilor de citire
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) { 
						fdmax = newsockfd;
					}

					//clientTCP_map[newsockfd]= new struct subscriber;
					subscriber sub;
					sub.ip=inet_ntoa(cli_addr.sin_addr);
					sub.port=ntohs(cli_addr.sin_port);
					clientTCP_map[newsockfd]=sub;
					
				} 
				else if(i==sockUDP){//se primeste mesaj pe socketul TCP
					memset(&serv_addr, 0, sizeof(serv_addr));
					memset(&cli_addr, 0, sizeof(cli_addr));
					memset(buffer,0,BUFLEN);
					clilen = sizeof(cli_addr);
					n=recvfrom(sockUDP,buffer,sizeof(buffer),0,(struct sockaddr *) &cli_addr,&clilen);
					DIE(n<1, "recv from UDP");
					memset(&mes,0,sizeof(mes));
					memset(&pack,0,sizeof(pachet));
					memcpy(&mes,buffer,sizeof(mes));
					if((mes.tip==0 && n>56)||(mes.tip==1 && n>53)||(mes.tip==2 && n>57)||(mes.tip==0 && n>1551)){
						continue;
					}
					
					if(topic_map.find(mes.topic)==topic_map.end()){
						topic_map[mes.topic]=unordered_map<string,int>();
					}

					//se creaza mesajul ce trebuie trimis la subscriber
					int a;
					unsigned short b;
					int c;
					char semn;
					char aux[1501];
					string mesaj_1="";
					string mesaj="";
					mesaj+=inet_ntoa(cli_addr.sin_addr);
					mesaj+=":";
					mesaj+=to_string(ntohs(cli_addr.sin_port));
					mesaj+=" - ";
					mesaj+=mes.topic;
					mesaj+=" - ";

					switch(mes.tip){
						case 0:
							semn = mes.payload[0]==0?'+':'-';
							memcpy(&a,mes.payload+1,4);
							a=ntohl(a);
							mesaj+="INT - ";
							mesaj+=semn;
							mesaj+=to_string(a);
							break;
						case 1:
							memcpy(&b,mes.payload,2);
							b=ntohs(b);
							mesaj+="SHORT_REAL - ";
							mesaj+=to_string(b/100);
							mesaj+=".";
							mesaj+=to_string((b%100)/10);
							mesaj+=to_string(b%10);
							break;
						case 2:
							semn = mes.payload[0]==0?'+':'-';
							memcpy(&a,mes.payload+1,4);
							a=ntohl(a);
							c=(int)mes.payload[5];
							mesaj_1+=to_string(a);
							while(mesaj_1.length()<c+1){
								mesaj_1.insert(0,"0");
							}
							if(c!=0){
								mesaj_1.insert(mesaj_1.length()-c,".");
							}
							mesaj+="FLOAT - ";
							mesaj+=semn;
							mesaj+=mesaj_1;
							break;
						case 3:
							memset(&aux,0,sizeof(aux));
							memcpy(aux,mes.payload,1500);
							mesaj+="STRING - ";
							mesaj+=aux;
							break;
					}
					
					string acs=to_string(mesaj.size());
					while(acs.length()<4){
						acs.insert(0,"0");
					}
					memcpy(pack.len,acs.c_str(),4);
					memcpy(pack.mes,mesaj.c_str(),mesaj.size());
					memset(&stor,0,sizeof(stored));
					st=0;
					//se trimite mesajul catre subscriber
					for(auto x:topic_map[mes.topic]){
						if(clientACT.find(x.first)!=clientACT.end()){
							for(auto y:clientTCP_map){
								if(y.second.id==x.first){
									int sr=send(y.first,&pack,mesaj.size()+4,0);
									DIE(sr<0,"send error");
								}
							}
						}
						else if(x.second==1){// in cazul in care subscriber-ul e deconectat
							if(st==0){		//si are SF 1 atunci se adauga mesajul in storage
								stor.pack=pack;
								st=1;
							}
							stor.cli.insert(x.first);
							
						}
					}
					if(st==1){
						storage[sid++]=stor;
					}
					
					mesaj.clear();
					memset(&pack,0,sizeof(pachet));
				}
				else {//se primesc comanzi de la un client
					memset(buffer, 0, BUFLEN);
					n = recv(i, buffer, sizeof(buffer), 0);
					DIE(n < 0, "recv");
					char aux[BUFLEN];
					memcpy(aux,buffer,BUFLEN);


					if (n == 0) {
						printf("Client %s disconnected\n",clientTCP_map[i].id.c_str());
						clientACT.erase(clientTCP_map[i].id);
						clientTCP_map.erase(i);
						close(i);
						FD_CLR(i, &read_fds);
					}
					memset(&name,0,sizeof(name));
					memset(&ip,0,sizeof(ip));
					memset(&port,0,sizeof(port));
					rem=0;
					len=0;
					s+=buffer;
					while(!s.empty()){

						if(saux.empty()){
							if(s.size()>=4-slen.size()){
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

						if(s.size()>=stoi(slen)-saux.size()){
							rem=saux.size();
							saux+=s.substr(0,stoi(slen)-saux.size());
							comanda com;
							memset(&com,0,sizeof(com));
							memcpy(com.com,saux.c_str(),saux.size());
							comenzi.insert(comenzi.end(),com);
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
					
					while(!comenzi.empty()){//se executa comenzile
					memset(buffer,0,BUFLEN);
					memcpy(buffer,comenzi[0].com,strlen(comenzi[0].com));
					comenzi.erase(comenzi.begin());	
					//daca se primesc informatiile unui client nou
					if (strncmp(buffer, "new", 3) == 0) {
						sscanf(buffer,"%*s %s %s %s",(char *)&name,(char *)&ip,(char *)&port);
						if(clientACT.find(name)==clientACT.end()){
							clientTCP_map[i].id=name;
							clientACT.insert(name);
							printf("New client %s connected from %s:%d\n",clientTCP_map[i].id.c_str(),clientTCP_map[i].ip.c_str(),clientTCP_map[i].port);
							//se verifica daca exista vreun pachet pentru acest client si se trimite
							for(auto x = storage.begin(); x!=storage.end();){

								if(x->second.cli.find(name)!=x->second.cli.end()){
									int sr = send(i,&x->second.pack,strlen(x->second.pack.mes)+4,0);
									DIE(sr<0,"send error");
									x->second.cli.erase(name);
								}
								if(x->second.cli.empty()){
										x=storage.erase(x);
								}
								else{
									x++;
								}
							}
							
						}
						else{
							//exista deja un client conectat cu id-ul dat
							feedback("",i,4);
							close(i);
							FD_CLR(i, &read_fds);
						}
						continue;

					}
					//comanda subscribe
					else if(strncmp(buffer,"subscribe",9)==0){
						memset(&topic,0,sizeof(topic));
						sscanf(buffer,"%*s %s %d",topic,&SF);
						topic_map[topic][clientTCP_map[i].id]=SF;	
						feedback(topic,i,0);
					}
					//comanda unsubscribe
					else if(strncmp(buffer,"unsubscribe",11)==0){
						memset(&topic,0,sizeof(topic));
						sscanf(buffer,"%*s %s",topic);
						if(topic_map.find(topic)!=topic_map.end()){
							if(topic_map[topic].find(clientTCP_map[i].id)!=topic_map[topic].end()){
								n=topic_map[topic].erase(clientTCP_map[i].id);
								//se confirma unsubscribe
								feedback(topic,i,1);

							}
							else{
								//nu era conectat la acest topic
								feedback(topic,i,2);
							}
						}
						else{
							//topicul nu exista
							feedback(topic,i,3);
						}
					}
					//se cere inchiderea conectiunii
					else if(strncmp(buffer,"exit",4)==0){
						printf("Client %s disconnected\n",clientTCP_map[i].id.c_str());
						clientACT.erase(clientTCP_map[i].id);
						clientTCP_map.erase(i);
						close(i);
						FD_CLR(i, &read_fds);
					}
					}
					
				}
			}
		}
	}
	//se inchid ultimii socketi ramasi
	n=close(sockTCP);
	DIE(n<0,"close TCP");
	n=close(sockUDP);
	DIE(n<0,"close UDP");
	return 0;
}
