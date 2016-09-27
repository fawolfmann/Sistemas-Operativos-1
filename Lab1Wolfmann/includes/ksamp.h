#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFSIZE 256  //lo defino como una longitud arbitraria de longitudes de lineas de texto extraidas de archivos

void cabecera();
void printEquipo();
void printActual();
void finder(char*, char*, char*);