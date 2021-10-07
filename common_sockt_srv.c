#include "common_sockt_srv.h"
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define ERROR_NO -1
#define ADDINFO_ERROR_MSG "getaddrinfo error"
#define CREAR_ERROR_MSG "falla al crear socket"
#define BIND_ERROR_MSG "falla de bind"
#define LISTEN_ERROR_MSG "falla al escuchar"
#define ACCEPT_ERROR_MSG "falla en aceptar"

int sockt_srv_init(Sockt_srv *skt, char *port, int max_q){
	struct addrinfo hints;
	struct addrinfo *ptr;
	int err = 0;
	int fd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, port, &hints, &ptr);
	if (err != 0) {
    	perror(ADDINFO_ERROR_MSG);
      return ERROR_NO;
   }
	fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (fd == -1) {
		perror(CREAR_ERROR_MSG);
		freeaddrinfo(ptr);
		return ERROR_NO;
	}

	err = bind(fd, ptr->ai_addr, ptr->ai_addrlen);
	if (err == -1) {
    	perror(BIND_ERROR_MSG);
    	close(fd);
    	freeaddrinfo(ptr);
    	return ERROR_NO;
	}
	freeaddrinfo(ptr);

	err = listen(fd, max_q);
	if (err == -1) {
    	perror(LISTEN_ERROR_MSG);
    	close(fd);
    	return ERROR_NO;
	}

   skt->fd = fd;
   return 0;
}

int sockt_srv_accept(Sockt_srv *skt){
	struct sockaddr_in *cli_act;
	unsigned int len = (socklen_t)sizeof(cli_act);
	int new_fd;
	new_fd = accept(skt->fd,(struct sockaddr *)&cli_act, &len);
	if (new_fd == -1){
		perror(ACCEPT_ERROR_MSG);
	}
	return new_fd;
}


void sockt_srv_destroy(Sockt_srv *skt){
	shutdown(skt->fd, SHUT_RDWR);
	close(skt->fd);
}
