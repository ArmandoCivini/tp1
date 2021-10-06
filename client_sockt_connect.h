#ifndef __CLIENT_SOCKT_CONNECT_H__
#define __CLIENT_SOCKT_CONNECT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

typedef struct Sockt_connect{
	struct addrinfo *resultado;
} Sockt_connect;

int sockt_connect_init(Sockt_connect *skt, char *host, char *port);

int sockt_connect_connection(Sockt_connect *skt);

void sockt_connect_destroy(Sockt_connect *skt);

#endif
