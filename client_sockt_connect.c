#include "client_sockt_connect.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdbool.h>
#define h_addr  h_addr_list[0]



int sockt_connect_init(Sockt_connect *skt, char *host, char *port){
	struct addrinfo hints;
	struct addrinfo *resultado;
	int err = 0;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0; 

	err = getaddrinfo(host, port, &hints, &resultado);

	if (err != 0) {
    	perror("getaddrinfo error");
    	freeaddrinfo(resultado);
    	return -1;
	}

	skt->resultado = resultado;
	return 0;

}

int sockt_connect_connection(Sockt_connect *skt){
	int fd;
	int err = 0;
	bool conexion_exitosa = false;
	struct addrinfo *ptr = skt->resultado;

	while (ptr != NULL && conexion_exitosa == false){
   		fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    	if (fd == -1) {
    		ptr = ptr->ai_next;
        	continue;
   		}
   		err = connect(fd, ptr->ai_addr, ptr->ai_addrlen);
   		if (err == -1) {
        	close(fd);
        	ptr = ptr->ai_next;
        	continue;
        }
        conexion_exitosa = true;
   	}

   	if (conexion_exitosa == false){
   		perror("falla de conexion");
   		return -1;
   	}
   	return fd;
}


void sockt_connect_destroy(Sockt_connect *skt){
	freeaddrinfo(skt->resultado);
}
