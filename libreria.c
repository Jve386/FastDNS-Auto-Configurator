// Incluimos las librerías necesarias para el correcto funcionamiento del programa

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <sys/types.h>
#include <iphlpapi.h>
#include <conio.h>
#include <time.h>

// Incluimos nuestro propio archivo de cabecera con las definiciones y prototipos necesarios
#include "Libreria.h"

// Definimos una macro para evitar las advertencias por parte del compilador relacionadas con la función sprintf
#define _CRT_SECURE_NO_WARNINGS

// Definimos constantes para los tamaños máximos de algunas variables
#define MAX_RUTA_ARCHIVO 256
#define MAX_ADAPTADORES 10
#define MAX_NUM_DNS 10
#define MAX_IP_LENGTH 16

// Agregamos un comentario para indicar que es una librería que debe ser enlazada en tiempo de ejecución
#pragma comment(lib, "ws2_32.lib")

// Definimos una estructura para almacenar información acerca de los servidores DNS


// Definimos una función que se encarga de leer un archivo de texto y almacenar su contenido en un array de caracteres
void readFile(char filePath[], char res[]) {

   FILE* file;

   file = fopen(filePath, "rt"); // Abrimos el archivo en modo lectura de texto

   if (file) { // Verificamos que se haya podido abrir el archivo

       char buffer[100]; 

       fgets(buffer, 100, file);  // Leemos una línea del archivo y la almacenamos en el buffer

       while (!feof(file)) {  // Iteramos hasta el final del archivo

           strcat(res, buffer); // Concatenamos el contenido del buffer con el array de caracteres resultado

           fgets(buffer, 100, file);  // Leemos la siguiente línea del archivo y la almacenamos en el buffer

       }



       fclose (file); // Cerramos el archivo

   } else {

       printf("Ha ocurrido un error en la localizacion del archivo.\n"); // Si no se pudo abrir el archivo, mostramos un mensaje de error

   }
}

/*
Esta función se encarga de preguntar al usuario el nombre del adaptador de red que desea consultar
para posteriormente mostrar la información de los servidores DNS asociados a dicho adaptador.
También obtiene el servidor DNS que está actualmente en uso en el equipo y lo guarda en dos archivos diferentes.
Requiere las librerías <stdio.h>, <string.h> y <stdlib.h>.
*/
void ConfigAdaptadores() {
    char str[255];
    char line[255];
    char adaptador[255];
    char dns[255];

    // Se ejecuta el comando "netsh interface ipv4 show interfaces" para obtener la lista de adaptadores de red
    FILE* f = _popen("netsh interface ipv4 show interfaces", "r");
    if (f == NULL) {
        printf("Error.");
        exit(1);
    }

    // Se imprime la lista de adaptadores obtenida
    printf("Los adaptadores de red disponibles son los siguientes:\n\n");
    while (fgets(line, sizeof line, f) != NULL) {
        printf("%s", line);
    }

    // Se solicita al usuario que ingrese el nombre del adaptador a consultar
    printf("Escriba el nombre del adaptador del que quiere consultar: ");
    leerCadena(adaptador, 100);
    _pclose(f);
    printf("\n\n");

    // Se genera el comando para obtener la información de los servidores DNS asociados al adaptador ingresado
    strcpy(str, "netsh interface ipv4 show dnsservers ");
    strcat(str, adaptador);
    system(str);

    // Se abre el archivo "dnsEscogido.tmp" en modo de escritura para guardar el servidor DNS Escogido por el usuario
    FILE* archivo = fopen("dnsEscogido.tmp", "a"); 
    if (archivo == NULL) {
        printf("Error al abrir el archivo dnsEscogido.tmp");
        exit(1);
    }

    // Se ejecuta el comando "ipconfig /all" para obtener la lista de adaptadores de red y sus propiedades
    FILE* adaptadores = _popen("ipconfig /all", "r");
    if (adaptadores == NULL) {
        printf("Error al obtener la lista de adaptadores de red");
        exit(1);
    } 

    // Se recorre línea por línea la salida del comando "ipconfig /all"
    char linea[256]= "";
    while (fgets(linea, sizeof(linea), adaptadores) != NULL) {
        // Si la línea contiene la cadena "Servidores DNS", se extrae el servidor DNS asociado al adaptador actual
        if (strstr(linea, "Servidores DNS") != NULL) {

            char* dnsStart = strstr(linea, ":") + 2;
            char* dnsEnd = strchr(dnsStart, '\n');
            if (dnsEnd != NULL) {
                *dnsEnd = '\0';
            }

            // Se guarda el servidor DNS en la variable "dns"
            strcpy(dns, dnsStart);

            // Se elimina cualquier espacio en blanco al inicio de la cadena "dns"
            char* dnsTrimmed = dns;
            while (*dnsTrimmed == ' ') {
                dnsTrimmed++;
            }            
        }   
    } 

    // Se cierran los archivos "adaptadores" y "archivo"
    fclose(adaptadores);
    fclose(archivo);

    // Se genera el comando para hacer un ping al servidor DNS Escogido y se guarda la salida en el archivo "dnsEscogido.tmp"
    char comando[512];
    // Se escribe el servidor DNS Escogido en el archivo dnsEscogido.tmp
    fprintf(archivo, "%s\n", dns);

    // Se construye el comando de ping a ejecutar para obtener la IP del servidor DNS Escogido
    sprintf(comando, "ping -n 1 %s > dnsEscogido.tmp", dns);

    // Se ejecuta el comando de ping y se redirige la salida a dnsEscogido.tmp
    system(comando);

    // Se crea el archivo ipDnsEscogido.tmp para almacenar la dirección IP del servidor DNS Escogido
    FILE* file = fopen("ipDnsEscogido.tmp", "w");

    // Se escribe la dirección IP del servidor DNS Escogido en el archivo ipDnsEscogido.tmp
    fprintf(file, dns);

    // Se cierra el archivo
    fclose(file);
}

/* Función para leer una cadena de caracteres desde la entrada estándar.
Recibe como argumentos un puntero a char para almacenar la cadena leída y un entero para especificar el tamaño máximo de la cadena.
Retorna un entero que indica si se pudo leer la cadena correctamente (1) o si hubo un error en la entrada (0). */

int leerCadena(char* cad, int n) {
    int i, c;
    c = getchar(); // Lee el primer carácter de la entrada
    if (c == EOF) { // Verifica si se llegó al final de la entrada (fin de archivo)
        cad[0] = '\0'; // Si es así, asigna un carácter nulo a la cadena y retorna 0 indicando error.
    return 0;
    }
    if (c == '\n') // Si el primer carácter leído es un salto de línea, asigna una cadena vacía.
        i = 0;
        else { // Si el primer carácter no es un salto de línea, lo asigna a la cadena y comienza a leer el resto de la entrada.
            cad[0] = c;
            i = 1;
    }
    for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++) // Lee caracteres adicionales hasta que se alcance el tamaño máximo de la cadena, se llegue al fin de la entrada o se encuentre un salto de línea.
        cad[i] = c;
        cad[i] = '\0'; // Asigna un carácter nulo al final de la cadena para indicar su final.
        if (c != '\n' && c != EOF) // Si no se alcanzó el final de la entrada, lee el resto de la línea actual y lo descarta.
            while ((c = getchar()) != '\n' && c != EOF);
    return 1; // Retorna 1 indicando que la cadena se leyó correctamente.
}

/*
Esta función tiene como objetivo leer un archivo que contiene una lista de direcciones IP de servidores DNS.
Luego, se realiza una verificación de conexión a cada una de estas direcciones usando el comando "ping".
Si se establece conexión, se muestra un mensaje de que la dirección ha sido incluida.
Además, se guarda la información de las direcciones que se hayan conectado satisfactoriamente en un archivo temporal "ping.tmp".
*/
void ComprobarAcces()
{
FILE* archivo = NULL; // Puntero al archivo DNSips.txt
FILE* archivoTemporal = NULL; // Puntero al archivo temporal ping.tmp
char linea[512]; // Variable donde se almacena cada línea del archivo de DNS
char rutaArchivo[255] = "./DNSips.txt"; // Ruta del archivo de DNS
char rutaArchivoTemporal[255] = "./ping.tmp"; // Ruta del archivo temporal

    // Se abre el archivo DNSips.txt en modo lectura
    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) { // Si no se pudo abrir el archivo, se muestra un mensaje de error y se sale de la función
        printf("Error: no se pudo abrir el archivo %s\n", rutaArchivo);
        exit(1);
    }

    // Se lee cada línea del archivo de DNS
    while (fgets(linea, sizeof(linea), archivo) != NULL) {

        if (linea[strlen(linea) - 1] == '\n') { // Si la línea tiene un salto de línea al final, se elimina
            linea[strlen(linea) - 1] = '\0';
        }

        // Se construye el comando ping para cada dirección IP del archivo de DNS
        char comando[512];
        sprintf(comando, "ping -n 1 %s >> infoPing.tmp", linea);

        // Se ejecuta el comando ping para cada dirección IP del archivo de DNS y se verifica si se estableció conexión
        int resultado = system(comando);

        if (resultado == 0) { // Si se estableció conexión, se muestra un mensaje de que la dirección ha sido incluida y se guarda en el archivo temporal ping.tmp
            printf("Dirección incluida:  %s \n", linea); 
            sprintf(comando, "ping -n 1 %s >> ping.tmp", linea);
            int resultado = system(comando);
        }
    }

    fclose(archivo); // Se cierra el archivo DNSips.txt

    // Se muestra un mensaje de finalización de la verificación y se indica la ubicación del archivo temporal ping.tmp
    printf("La verificacion de servidores DNS ha finalizado.\n"
        "Los servidores DNS accesibles se encuentran en el archivo temporal: %s\n", rutaArchivoTemporal);
    
}
/** Esta función busca en el archivo temporal de ping para encontrar los tiempos de respuesta de los servidores DNS que se incluyeron en la lista. Luego, busca el tiempo de respuesta más bajo para determinar qué servidor DNS es el más rápido. Finalmente, imprime el servidor DNS más rápido y su tiempo de respuesta.

El programa lee cada línea del archivo temporal de ping y busca las líneas que contienen la cadena "TTL=", lo que indica que la respuesta de ping fue exitosa.  **/

void TestSpeedDNS()  {
    FILE* archivo = NULL; // variable para el archivo
    char linea[512]; // variable para almacenar cada línea del archivo
    char rutaArchivo[256] = "./ping.tmp"; // ruta del archivo temporal de ping
    int tiempo[10], i = 0; // array para almacenar los tiempos de respuesta y un contador

    // inicialización de los tiempos de respuesta en un valor alto (999999)
    for (int i = 0; i < 10; i++)
    {
        tiempo[i] = 999999;
    }

    // apertura del archivo de ping
    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", rutaArchivo);
        exit(1); // salida del programa si no se puede abrir el archivo
    }

    // impresión del encabezado para mostrar los tiempos de respuesta
    printf("Tiempo de respuesta de los servidores DNS:\n\n");

    // lectura de cada línea del archivo de ping
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // si la línea contiene "TTL=", significa que es una respuesta de ping exitosa
        if (strstr(linea, "TTL=") != NULL) {
            char* token = strtok(linea, " ="); // separar la línea en tokens usando espacio o signo igual como delimitadores
            while (token != NULL) {
                // si el token es "tiempo", el siguiente token contiene el tiempo de respuesta
                if (strncmp(token, "tiempo", 6) == 0) {
                    token = strtok(NULL, " ="); // avanzar al siguiente token
                    token[strlen(token) - 2] = '\0'; // eliminar los caracteres "ms" del final del tiempo de respuesta
                    int tiempoRespuesta = atoi(token); // convertir el tiempo de respuesta a un entero
                    tiempo[i] = tiempoRespuesta; // agregar el tiempo de respuesta al array de tiempos
                    i++; // incrementar el contador
                }
                token = strtok(NULL, " ="); // avanzar al siguiente token
            }
        }
    }
    fclose(archivo); // cerrar el archivo de ping

    // encontrar el tiempo de respuesta más bajo y el índice correspondiente (que indica qué servidor DNS es el más rápido)
    int minTiempo = tiempo[0];
    int dnsMasRapido = 0;
    for (i = 1; i < 10; i++) {
        if (tiempo[i] < minTiempo) {
            minTiempo = tiempo[i];
            dnsMasRapido = i;
        }
    }

    // imprimir el servidor DNS más rápido y su tiempo de respuesta
    printf("El servidor DNS mas rapido es el %d, con un tiempo de respuesta de %dms\n", dnsMasRapido + 1, minTiempo);
}

/*
Esta función busca optimizar los servidores DNS disponibles según el tiempo de respuesta y la cantidad de saltos necesarios.
Para ello, se lee el archivo "ping.tmp" y se extrae la información de tiempo de respuesta y cantidad de saltos de cada servidor.
Luego, se compara el tiempo de respuesta de cada servidor y se guarda el índice del más rápido en la variable "dnsMasRapido".
En caso de empate, se compara la cantidad de saltos necesarios para alcanzar el servidor y se selecciona el que requiere menos saltos.
Si no se puede determinar el servidor DNS más rápido, se muestra un mensaje de error.
*/
void ComparativaSaltos() {
    FILE* archivo = NULL;
    char linea[512];
    char rutaArchivo[256] = "./ping.tmp";
    int tiempo[10], saltos[10], i = 0;

    // Inicializamos los arreglos de tiempo y saltos con valores altos para facilitar la comparación.
    for (i = 0; i < 10; i++) {
        tiempo[i] = 999999;
        saltos[i] = 99;
    }

    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", rutaArchivo);
        exit(1);
    }

    printf("Tiempo de respuesta y saltos de los servidores DNS:\n\n");

    // Leemos el archivo "ping.tmp" y extraemos la información de tiempo de respuesta y cantidad de saltos de cada servidor DNS.
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "TTL=") != NULL) {
            char* token = strtok(linea, " =");
            while (token != NULL) {
                // Si el token corresponde al tiempo de respuesta, lo extraemos y lo guardamos en el arreglo "tiempo".
                if (strncmp(token, "tiempo", 6) == 0) {
                    token = strtok(NULL, " =");
                    token[strlen(token) - 2] = '\0';
                    int tiempoRespuesta = atoi(token);
                    tiempo[i] = tiempoRespuesta;
                } 
                // Si el token corresponde a la cantidad de saltos necesarios, lo extraemos y lo guardamos en el arreglo "saltos".
                else if (strncmp(token, "salto", 5) == 0) {
                    token = strtok(NULL, " =");
                    token[strlen(token) - 1] = '\0';
                    int saltosNecesarios = atoi(token);
                    saltos[i] = saltosNecesarios;
                }
                token = strtok(NULL, " =");
            }
            i++;
        }
    }

    fclose(archivo);


    // Buscamos el servidor DNS con menor tiempo de respuesta
    int minTiempo = tiempo[0]; // Asignamos el primer tiempo de respuesta como el mínimo
    int dnsMasRapido = 0; // Asignamos el primer servidor DNS como el más rápido
    for (i = 1; i < 10; i++) { // Iteramos sobre los demás servidores DNS
        if (tiempo[i] < minTiempo) { // Si el tiempo de respuesta es menor al mínimo
            minTiempo = tiempo[i]; // Lo actualizamos como el nuevo mínimo
            dnsMasRapido = i; // Actualizamos el servidor DNS más rápido
        }
    }

    // Buscamos el servidor DNS con menor cantidad de saltos si hay varios con el mismo tiempo de respuesta
    for (i = 0; i < 10; i++) { // Iteramos sobre todos los servidores DNS
        if (tiempo[i] == minTiempo && saltos[i] < saltos[dnsMasRapido]) { // Si el tiempo de respuesta es igual al mínimo y los saltos son menores al actual más rápido
            dnsMasRapido = i; // Actualizamos el servidor DNS más rápido
        }
    }

    // Mostramos el servidor DNS más rápido y su información
    if (minTiempo == 999999) { // Si no se encontró un servidor DNS
        printf("No se pudo determinar el servidor DNS más rápido.\n");
    } else { // Si se encontró un servidor DNS
        printf("El servidor DNS más rápido es el %d, con un tiempo de respuesta de %dms y %d saltos.\n", dnsMasRapido + 1, minTiempo, saltos[dnsMasRapido]);
    }
}

// No funciona

// void obtenerSaltos(const char* direccionIP, int* numSaltos, FILE* archivoSaltos) {
//     char comando[256];
//     sprintf(comando, "tracert -d %s", direccionIP);

//     FILE* pipe = _popen(comando, "r");
//     if (pipe == NULL) {
//         printf("Error: no se pudo ejecutar el comando tracert.\n");
//         exit(1);
//     }

//     char linea[256];
//     while (fgets(linea, sizeof(linea), pipe) != NULL) {

//         // Guardar la línea en el archivo temporal
//         fprintf(archivoSaltos, "%s", linea);

//         // Buscar la línea que contiene el número de salto
//         if (strstr(linea, "ms") != NULL) {
//             *numSaltos = atoi(linea);
//             break;
//         }
//     }

//     _pclose(pipe);
// }