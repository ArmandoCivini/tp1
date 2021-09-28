#ifndef __SERVER_AHORCADO_H__
#define __SERVER_AHORCADO_H__

#include <string.h>
#include <stdbool.h>

typedef struct Ahorcado {
	char *pal;
	int len;
	int intentos;
	char *revelados;
} Ahorcado;

void ahorcado_init(Ahorcado *aho, const char *p, int intent);

bool ahorcado_probar(Ahorcado *aho, const char c, char *revel, int *intent);

void ahorcado_destroy(Ahorcado *aho);

#endif
