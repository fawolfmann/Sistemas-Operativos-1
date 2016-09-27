#include "./includes/ksamp.h"
#include "./includes/parteB.h"
#include "./includes/parteC.h"

/**
 * Llama a las funciones que imprimen lo solicitado en la parte C del trabajo práctico.
 */
void parteC(){
	printf("\n");
	cpuTimes();
	contextChanges();
	bootUpTime();
	processes();
	return;
}

/**
 * Imprime el tiempo de cpu de usuario, sistema, e idle.
 */
void cpuTimes(){
	float user, sys, idle;
	char tUser[15], tSys[15], tIdle[15];
	FILE *fd;
	fd = fopen("/proc/stat","r");
	fscanf(fd,"cpu %f %*f %f %f", &user, &sys, &idle);
	secToDayHourMinSec(user/100, tUser);
	printf("Tiempo de cpu para el usuario: %s \n",tUser);
	secToDayHourMinSec(sys/100, tSys);
	printf("Tiempo de cpu para el sistema: %s \n",tSys);
	secToDayHourMinSec(idle/100, tIdle);	
	printf("Tiempo de cpu idle: %s \n",tIdle);
	fclose(fd);
	return;
}

/**
 * Imprime la cantidad de cambios de contexto
 */
void contextChanges(){
	char resultado[256];
	unsigned int cambios;
	
	finder("/proc/stat", resultado, "ctxt");
	sscanf(resultado, "ctxt %u", &cambios);
	
	printf("Cambios de contexto: %u \n", cambios);
	return;
}

/**
 * Imprime fecha y hora de inicio del sistema
 */ 
void bootUpTime(){
	char resultado [BUFFSIZE];
	time_t btime;
	unsigned int aux;
	char booted[40];
			
	finder("/proc/stat", resultado , "btime");
	sscanf(resultado, "btime %u", &aux);
	btime= (time_t)aux;
	
	//formatea el tiempo a un formato específico.
	strftime(booted,sizeof(booted),"%c", localtime(&btime));
	printf("Boot Up Time: %s \n", booted);
	return;
}

/**
 * Imprime la cantidad de procesos creados desde el inicio del sistema
 */
void processes(){
	char resultado[BUFFSIZE];
	int procesos;
		
	finder("/proc/stat", resultado, "processes");
	sscanf(resultado, "processes %u", &procesos);
	
	printf("Cantidad de procesos creados: %u\n", procesos);
	return;
}