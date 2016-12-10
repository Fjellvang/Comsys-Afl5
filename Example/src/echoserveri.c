/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#define MAXCONNECTIONS (20)

void echo(int connfd);
void init_connect_list();
int add_to_connect_list(char *client_hostname, char *client_port);
void print_connected_list();

typedef struct{
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];
    int active;
} connected_t;

connected_t connected_list[MAXCONNECTIONS];

int main(int argc, char **argv) 
{
    init_connect_list();
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

        listenfd = Open_listenfd(argv[1]);
    Fork();
    while (1) {
    	clientlen = sizeof(struct sockaddr_storage); 
    	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        add_to_connect_list(client_hostname, client_port);	
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        print_connected_list();
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
void init_connect_list(){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        connected_list[i].active = -1;
    }

}

int add_to_connect_list(char *client_hostname, char *client_port){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == -1){
            connected_list[i].active = 1;
            *connected_list[i].client_hostname = *client_hostname;
	    *connected_list[i].client_port = *client_port;
	    return 1;
        }
    }
    // not enough space
    printf("NO MOAR SPACE MAN \n");
    return -1;
}

void print_connected_list(){
    for(int i = 0; i < MAXCONNECTIONS; i++){
        if(connected_list[i].active == 1){
            printf("Connected to (%s, %s)\n", connected_list[i].client_hostname, connected_list[i].client_port);
        }
    }
}
