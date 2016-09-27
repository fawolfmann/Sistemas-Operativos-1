#include "./includes/ksamp.h"
#include "./includes/parteD.h"

/**
 * Llama a las funciones que imprimen lo solicitado en la parte D del trabajo práctico.
 * @param a Indica cada cuantos segundos se vuelve a evaluar lo solicitado
 * @param b Indica durante cuantos segundos debe hacerse. luego b/a indica cuantos ciclos de a segundos deben realizarse
 */ 
void parteD(int a, int b){	
	int i;
	i=b/a;
	printf("-------------------------------------\n");
	while(i>0){
		pedidosHDD();
		memInfo();
		loadAvg();
		i--;
		sleep(a);
		printf("-------------------------------------\n");
	}
	return;
}

/**
 * Imprime la cantidad de pedidos a disco realizados
 */
void pedidosHDD(){
	char resultado[BUFFSIZE];
	unsigned int lecturas, escrituras, pedidos;
	finder("/proc/diskstats", resultado, "sda");
	sscanf(resultado, "sda %u", &lecturas);
	sscanf(resultado, "sda %*u %*u %*u %*u %u", &escrituras);
	pedidos = escrituras + lecturas;
	printf("Cantidad de pedidos al disco: %u\n", pedidos);	
	return;
}

/**
 * Imprime la informacion de la memoria
 */
void memInfo(){
	char resultado[BUFFSIZE];
	unsigned int memTotal, memFree;
		
	finder("/proc/meminfo", resultado, "MemTotal");
	sscanf(resultado, "MemTotal: %u", &memTotal);	
	finder("/proc/meminfo", resultado, "MemFree");
	sscanf(resultado, "MemFree: %u", &memFree);	
	printf("Memoria total: %u MB\n", memTotal/1024);
	printf("Memoria disponible: %u MB\n", memFree/1024);
	return;
}

/**
 * Imprime lista de promedios de carga de un minuto
 */
void loadAvg(){
	FILE *fd;
	float load;			
	fd = fopen("/proc/loadavg","r");
	fscanf(fd,"%f", &load);			
	printf("Promedio de carga de un minuto: %f\n", load);
	fclose(fd);
	return;
}