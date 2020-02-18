#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

//functie care compune linia ce e folosita ca url_param (pt GET) sau ca body (pt POST)

char *compute_payload(JSON_Object *commit){
    char *s=calloc(BUFLEN,sizeof(char));
    int n=json_object_get_count(commit);//numarul de perechi nume-valoare
    for(int i=0; i<n; i++){
        if(strcmp(json_object_get_name(commit,i),"token")==0){
            continue;
        }//daca se gaseste un alt obiect json se apeleaza functia compute_payload pe acel obiect
        if(json_object_get_object(commit,json_object_get_name(commit,i))!=NULL){
            strcat(s,compute_payload(json_object_get_object(commit,json_object_get_name(commit,i))));
        }
        else{//concatenarea informatiilor
            strcat(s,json_object_get_name(commit,i)); //numele perechii
            strcat(s,"=");
            strcat(s,json_string(json_object_get_value_at(commit,i))); //valoarea perechii
            strcat(s,"&");
        }
    }
    return s;
}

//functie care compune message

char *compute_request(char *host, char* auth, char cookie[], JSON_Object *json){
    char *message = calloc(BUFLEN,sizeof(char));
    char line[BUFLEN];
    char buf[500];
    char *b=cookie;
    char s[BUFLEN]="";
    char cookie_line[BUFLEN]="";
    memset(s,0,BUFLEN);
    JSON_Object *commit=json;
    
    //se parseaza campurile din data
    int data=0;
    if(json_object_get_object(commit,"data")!=NULL){
        if(json_object_dotget_object(commit,"data.url")!=NULL){
            strcat(s,compute_payload(json_object_dotget_object(commit,"data.queryParams")));
        }
        else{
            strcat(s,compute_payload(json_object_get_object(commit,"data")));
        }
        s[strlen(s)-1]='\0';
        data=1;
    }
    if(json_object_get_string(commit,"data")!=NULL){
        strcat(s,json_object_get_string(commit,"data"));
    }

    //extrag method, url si type (daca exista)
    char *method=(char*)json_object_get_string(commit,"method");
    char *url=(char*)json_object_get_string(commit,"url");
    char *type=NULL;
    if(strcmp(method,"POST")==0){
        type=(char*)json_object_get_string(commit,"type");
    }
    //prima linie de header
    memset(line,0,BUFLEN);
    if(strcmp(method,"GET")==0){
        if(data==1){
            sprintf(line, "GET %s?%s HTTP/1.1", url, s);
        }
        else{
            sprintf(line, "GET %s HTTP/1.1", url);
        }
    }
    else if(strcmp(method,"POST")==0){
        sprintf(line,"POST %s HTTP/1.1",url);
    }

    //header de host
    compute_message(message,line);
    memset(line,0,BUFLEN);
    sprintf(line, "Host: %s",host);
    compute_message(message,line);


    //header de autentificare (daca e cazul)
    if(auth!=NULL){
        memset(line,0,BUFLEN);
        sprintf(line,"Authorization: Bearer %s",auth);
        compute_message(message,line);
    }

    //parsarea de cookie-uri si introducerea acestora in mesaj
    if(cookie!=NULL){
    while(1){
        b=strstr(b,"Set-Cookie");
        if(b==NULL){
            break;
        }
        memset(buf,0,500);
        memcpy(buf,b+12,strcspn(b, ";\n")-12);
        strcat(cookie_line,buf);
        strcat(cookie_line,";");
        b++;
    }
        cookie_line[strlen(cookie_line)-1]='\0';
        memset(line,0,BUFLEN);
        sprintf(line,"Cookie: %s",cookie_line);
        compute_message(message,line);
    }
    //se introd headere pentru Content-Type si Content-Length
    if(type!=NULL){
        memset(line,0,BUFLEN);
        sprintf(line,"Content-Type: %s",type);
        compute_message(message,line);
        memset(line,0,BUFLEN);
        sprintf(line,"Content-Length: %ld",strlen(s));
        compute_message(message,line);
    }
    //linie goala
    compute_message(message,"");

    //se pune body daca e tip POST
    if(strcmp(method,"POST")==0){
        compute_message(message,s);   
    }

    return message;
}
