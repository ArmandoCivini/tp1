#ifndef __COMMON_SOCKT_H__
#define __COMMON_SOCKT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

typedef struct Sockt{
	int fd;
} Sockt;

void sockt_init(Sockt *skt, int fd);

//lee del socket y garantiza leer exp_len bytes y los pone en el buf
int sockt_read(Sockt *skt, char *buf, size_t exp_len);

//escribe al socket y garantiza escribir exp_len bytes del buf
int sockt_write(Sockt *skt, char *buf, size_t exp_len);

void sockt_destroy(Sockt *skt);

#endif
