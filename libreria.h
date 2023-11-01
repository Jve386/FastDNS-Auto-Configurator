#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// Path: libreria.c
#ifndef LIBRERIA_H_INCLUDED
#define LIBRERIA_H_INCLUDED

typedef struct {
	char nombre[256];
	char descripcion[256];
} Adaptador;

typedef struct {
	char** servidores;
	int numServidores;
	
} DnsServidores;



void readFile(char filePath[], char res[]);
void ConfigAdaptadores();
int leerCadena(char* cad, int n);
void ComprobarAcces();
void TestSpeedDNS();
void ComparativaSaltos();
// void obtenerSaltos(const char* direccionIP, int* numSaltos, FILE* archivoSaltos)
#endif 