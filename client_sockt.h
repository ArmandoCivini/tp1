#ifndef __CLIENT_SOCKT_H__
#define __CLIENT_SOCKT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

typedef struct Sockt_cli{
	int fd;
} Sockt_cli;

void sockt_cli_init(Sockt_cli *skt, char *host, uint16_t port);

void sockt_cli_read(Sockt_cli *skt, char *buf, size_t exp_len);

void sockt_cli_write(Sockt_cli *skt, char *buf, size_t exp_len);

void sockt_cli_destroy(Sockt_cli *skt);

#endif