#ifndef __SERVER_SOCKT_H__
#define __SERVER_SOCKT_H__

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sockt_srv{
	int fd;
} Sockt_srv;

//max_q es el maximo numero de clientes en la cola
int sockt_srv_init(Sockt_srv *skt, char *port, int max_q);

//acepta un cliente de la cola y establece conexion
int sockt_srv_accept(Sockt_srv *skt);

void sockt_srv_destroy(Sockt_srv *skt);

#endif
