#include "./includes/ksamp.h"
#include "./includes/parteB.h"
#include "./includes/parteC.h"
#include "./includes/parteD.h"

int main(int argc, char **argv)
{
	int a, b, c;    
	opterr = 0; 
	short int flagC = 0;
	short int flagD = 0;
	
	while((c = getopt (argc, argv, "sl:"))!=-1){
		switch(c){
			case 's':
				if(argc==2){
					flagC = 1;
					break;
				}
				else{
					printf("Las opciones validas son -s , -l a b , siendo a y b numeros enteros.\n");
					return 0;
				}
			case 'l':
				if(argc == 4){
					flagC = 1;
					flagD = 1;
					break;
				}
				else{
					printf("Las opciones validas son -s , -l a b , siendo a y b numeros enteros.\n");
					return 0;
				}
			default:
				printf("Las opciones validas son -s ó -l a b , siendo a y b numeros enteros\n");
				return 0;						
		}
	}
	
	//La cabecera y la parte B se imprimen siempre.
	cabecera();
	parteB();
	
	if(flagC)
		parteC();
	if(flagD){
		sscanf(argv[2], "%u", &a);
		sscanf(argv[3], "%u", &b);
		parteC();
		parteD(a,b);
	}
	
	return 0;
}

/**
 * ejecuta las funciones que imprimen la cabecera del programa
 */
void cabecera(){
	printf("\n");
	printEquipo();
	printActual();
	return;
}

/**
 * Obtiene el nombre del equipo.
 */
void printEquipo(){
	FILE *fd; 
	char name[30];                                 
	fd = fopen("/proc/sys/kernel/hostname","r"); 
	fscanf(fd, "%s", name);					       
	printf("Nombre de la maquina: %s \n", name); 
	fclose(fd);
	return;
}

/**
 * Imprime la fecha y la hora actuales.
 */
void printActual(){
	char fecha[15]; 
	char hora[15];
	char resultado[BUFFSIZE];
	finder("/proc/driver/rtc", resultado, "rtc_date");
	sscanf(resultado, "rtc_date : %s", fecha);
	finder("/proc/driver/rtc", resultado, "rtc_time");
	sscanf(resultado, "rtc_time : %s", hora);
	printf("Fecha: %s   -   Hora: %s \n", fecha, hora);
	return;
}

/**
 * Coloca en resultado una linea que comienza con la cadena findedStr, recortada de una linea extraida de un archivo de texto.
 * @param filename El archivo en el que se va a buscar.
 * @param resultado Almacena la linea que comienza con la cadena findedStr.
 * @param findedStr Cadena a buscar.
 */
void finder(char* filename, char* resultado, char* findedStr){
	char buffer[BUFFSIZE]; 
	char* match = NULL;
	FILE* fd;
	fd = fopen(filename,"r");
	
	while(feof(fd) == 0){
		fgets(buffer, BUFFSIZE, fd);           
		match = strstr(buffer, findedStr);      
		if(match!=NULL) break;
	}
	
	fclose(fd);
	strcpy(resultado,match);
	return;
}