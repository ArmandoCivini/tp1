#ifndef __SERVER_H__
#define __SERVER_H__

#include "server.h"
#include "common_sockt_srv.h"
#include "common_sockt.h"
#include "common_ahorcado.h"
#include "common_protocolo.h"
#include <stdio.h>
#include <stdbool.h>


void empezar_juego(char *file, char *port, int intentos);

int main(int argc, char *argv[]);

#endif
