#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Libreria.h"

#define _CRT_SECURE_NO_WARNINGS



int main() {
    int opcion = 1;
    char filePath[255];

    while (opcion != 0) {
        printf("\nMenu:\n");
        printf("1. Indica ruta del archivo DNSips.txt para que muestre el contenido.\n");
        printf("2. Mostrar adaptadores de red, seleccionar deseado generar ipconfig.tmp.\n");
        printf("3. Comprobar si las IPs son accesibles; volcar resultado exitoso en ping.tmp.\n");
        printf("4. ¿Cuál es el DNS más rápido? \n");
        printf("5. Optimizar DNS y comprobar saltos. \n");
        // printf("6. Generar temp de saltos en DNSIPS.txt. \n");
        printf("0. Salir\n");

        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Se solicita la ruta del archivo
                printf("Indique la ruta del archivo que desee leer:\n");
                scanf ("%s", filePath);
                // Se lee el archivo             
                char dnsFile[1000] = "";
                readFile(filePath, dnsFile);
                // Se imprime el contenido del archivo
                printf("El contenido del fichero (%s) es:\n%s", filePath, dnsFile);
                break;
            case 2:
                ConfigAdaptadores();
                break;
            case 3:
                ComprobarAcces();
                break;
            case 4:
                TestSpeedDNS();
                break;
            case 5:
                ComparativaSaltos();
                break;
            // case 6: {
            //     char ruta_archivo[256];
            //     printf("Introduzca la ruta del archivo dnsips.txt: ");
            //     scanf("%s", ruta_archivo);

            //     FILE* archivo_ips = fopen(ruta_archivo, "r");
            //     if (archivo_ips == NULL) {
            //         printf("Error: no se pudo abrir el archivo dnsips.txt.\n");
            //         exit(1);
            //     }

            //     FILE* archivo_saltos = fopen("saltos.txt", "w+");
            //     if (archivo_saltos == NULL) {
            //         printf("Error: no se pudo crear el archivo saltos.txt.\n");
            //         exit(1);
            //     }

            //     char linea[256];
            //     while (fgets(linea, sizeof(linea), archivo_ips) != NULL) {
            //         int numSaltos = 0;
            //         obtenerSaltos(linea, &numSaltos);
            //         fprintf(archivo_saltos, "%s - %d saltos\n", linea, numSaltos);
            //     }

            //     fclose(archivo_ips);
            //     fclose(archivo_saltos);
            //     printf("Se ha generado el archivo saltos.txt con la información de los saltos de los DNS.\n");
            //     break;
            // }
            case 0:
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }
    }
}