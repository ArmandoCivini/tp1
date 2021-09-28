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

void sockt_srv_init(Sockt_srv *skt, uint16_t port, int max_q);

void sockt_srv_accept(Sockt_srv *skt);

void sockt_srv_read(Sockt_srv *skt, char *buf, size_t exp_len);

void sockt_srv_write(Sockt_srv *skt, char *buf, size_t exp_len);

void sockt_srv_destroy(Sockt_srv *skt);

#endif