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
	int fd_act;
} Sockt_srv;

//max_q es el maximo numero de clientes en la cola
void sockt_srv_init(Sockt_srv *skt, uint16_t port, int max_q);

//acepta un cliente de la cola y establece conexion
void sockt_srv_accept(Sockt_srv *skt);

//lee del socket y garantiza leer exp_len bytes y los pone en el buf
void sockt_srv_read(Sockt_srv *skt, char *buf, size_t exp_len);

//escribe al socket y garantiza escribir exp_len bytes del buf
void sockt_srv_write(Sockt_srv *skt, char *buf, size_t exp_len);

void sockt_srv_destroy(Sockt_srv *skt);

#endif
