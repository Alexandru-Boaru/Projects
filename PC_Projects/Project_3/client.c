#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

typedef struct{
    char name[LINELEN];
    char value[LINELEN];
}param; 


//functie care cauta adresa IP asociata unui nume de host
char* get_ip(char* host)
{
    struct addrinfo hints, *result, *p;
    int errcode;
    char *addrstr=calloc(100,sizeof(char));
    void *ptr;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    errcode = getaddrinfo(host,NULL,&hints,&result);
    if(errcode!=0){
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(errcode));
        exit(-1);
    }
    for(p=result;p!=NULL;p=p->ai_next){
        switch(p->ai_family){
            case AF_INET:
                ptr = &((struct sockaddr_in *)p->ai_addr)->sin_addr;
                break;
            case AF_INET6:
                ptr = &((struct sockaddr_in6 *)p->ai_addr)->sin6_addr;
        }
        inet_ntop(result->ai_family,ptr,addrstr,100);
        return addrstr;
    }
    return addrstr;
}

int main(int argc, char *argv[])
{
    char *message[6];
    char *response[6];
    char* url;
    char* token;
    
    char cookie[1000];
    int sockfd;
    JSON_Value *root_value;
    JSON_Object *commit;
    char *aux;


    //TASK 1
    root_value=json_value_init_object();
    commit=json_value_get_object(root_value);
    //am facut un obiect propriu pe care l-am trimis sa fie parsat
    json_object_set_string(commit,"url","/task1/start");
    json_object_set_string(commit,"method","GET");
    
    message[0]=compute_request(IP_SERVER,NULL,NULL,commit);
    printf("%s\n", message[0]);
    json_value_free(root_value);


    sockfd=open_connection(IP_SERVER,PORT_SERVER,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[0]);
    response[0]=receive_from_server(sockfd);
    printf("%s\n", response[0]);

    //TASK 2
    //extrag cookie-uri
    memset(cookie,0,1000);
    memcpy(cookie,strstr(response[0],"Set-Cookie"),strstr(response[0],"X-Response-Time")-strstr(response[0],"Set-Cookie"));

    //extrag obiectul json
    aux=strchr(response[0],'{');
    
    root_value = json_parse_string(aux);
    commit=json_value_get_object(root_value);

    message[1]=compute_request(IP_SERVER,NULL,cookie,commit);
    printf("%s\n", message[1]);
    close_connection(sockfd);
    sockfd=open_connection(IP_SERVER,PORT_SERVER,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[1]);
    response[1]=receive_from_server(sockfd);
    printf("%s\n", response[1]);

    //TASK 3
    //extrag cookie-uri
    memset(cookie,0,1000);
    memcpy(cookie,strstr(response[1],"Set-Cookie"),strstr(response[1],"X-Response-Time")-strstr(response[1],"Set-Cookie"));

    //extrag obiectul json
    aux=strchr(response[1],'{');
    
    root_value = json_parse_string(aux);
    commit=json_value_get_object(root_value);
    //extrag token-ul
    token=(char*)json_object_dotget_string(commit,"data.token");
    //adaug raspunsurile in obiectul json
    json_object_dotset_string(commit,"data.raspuns1","omul");
    json_object_dotset_string(commit,"data.raspuns2","numele");


    message[2]=compute_request(IP_SERVER,token,cookie,commit);
    printf("%s\n", message[2]);

    close_connection(sockfd);
    sockfd=open_connection(IP_SERVER,PORT_SERVER,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[2]);
    response[2]=receive_from_server(sockfd);
    printf("%s\n", response[2]);

    //TASK 4
    //extrag cookie-uri
    memset(cookie,0,1000);
    memcpy(cookie,strstr(response[2],"Set-Cookie"),strstr(response[2],"X-Response-Time")-strstr(response[2],"Set-Cookie"));

    //extrag obiectul json
    aux=strchr(response[2],'{');
    
    root_value = json_parse_string(aux);
    commit=json_value_get_object(root_value);

    message[3]=compute_request(IP_SERVER,token,cookie,commit);
    printf("%s\n", message[3]);

    close_connection(sockfd);
    sockfd=open_connection(IP_SERVER,PORT_SERVER,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[3]);
    response[3]=receive_from_server(sockfd);
    printf("%s\n", response[3]);

    //TASK 5 PART 1
    //extrag cookie-uri
    memset(cookie,0,1000);
    memcpy(cookie,strstr(response[3],"Set-Cookie"),strstr(response[3],"X-Response-Time")-strstr(response[3],"Set-Cookie"));

    //extrag obiectul json
    aux=strchr(response[3],'{');
    
    root_value = json_parse_string(aux);
    commit=json_value_get_object(root_value);

    
    //fac un obiect nou in care mut datele pentru operatia de GET
    JSON_Value* root_value_aux=json_value_init_object();
    JSON_Object* commit_aux=json_value_get_object(root_value_aux);
    JSON_Object* commit_aux1;
    JSON_Value* root_data=json_value_init_object();
    JSON_Object* data=json_value_get_object(root_data);

    url=strchr(json_object_dotget_string(commit,"data.url"),'/');
    json_object_set_string(commit_aux,"url",url);
    json_object_set_string(commit_aux,"method",(char*)json_object_dotget_string(commit,"data.method"));
    json_object_set_value(commit_aux,"data",root_data);
    commit_aux1=json_object_dotget_object(commit,"data.queryParams");
    int n=json_object_get_count(commit_aux1);
    for(int i=0; i<n; i++){
        json_object_set_string(data,
            (char*)json_object_get_name(commit_aux1,i),
            (char*)json_string(json_object_get_value_at(commit_aux1,i)));
    }

    char host[50];
    memset(host,0,50);
    memcpy(host,json_object_dotget_string(commit,"data.url"),strcspn(json_object_dotget_string(commit,"data.url"), "/"));
    
    message[4]=compute_request(get_ip(host),NULL,cookie,commit_aux);
    printf("%s\n", message[4]);
    close_connection(sockfd);
    sockfd=open_connection(get_ip(host),80,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[4]);
    response[4]=receive_from_server(sockfd);
    printf("%s\n", response[4]);

    //TASK 5 PART 2
    //scot campul data
    json_object_remove(commit,"data");
    //extrag obiectul din mesaj si il pun in obiectul anterior
    aux=strchr(response[4],'{');
    json_object_set_string(commit,"data",aux);
    
    message[5]=compute_request(IP_SERVER,token,cookie,commit);
    printf("%s\n", message[5]);
    close_connection(sockfd);
    sockfd=open_connection(IP_SERVER,PORT_SERVER,AF_INET,SOCK_STREAM,0);
    send_to_server(sockfd,message[5]);
    response[5]=receive_from_server(sockfd);
    printf("%s\n", response[5]);
    //eliberez message-urile si response-urile
    for(int i=0; i<6;i++){
        free(message[i]);
        free(response[i]);
    }
    close_connection(sockfd);

    return 0;
}
