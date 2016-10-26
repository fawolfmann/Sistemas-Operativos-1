#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>

int getPaths(char* paths []);
int leerTeclado(char* argv[], char* cadena);
void buscarArchivo(char* arch, char* paths[], char* execPath);
int background(char* argv[]);
int checkRedirect(char* argv[], char fileName[]);
int checkPipe(char* argv[], char* argv1[], char* argv2[]);
void outPut(char fileName[]);
void inPut(char fileName[]);
void doPipeline(char* argv1[], char* argv2[], char* paths[]);
struct passwd *pwd;

int main()
{
	int pid, flagWaitPID, bgProcess, pipeExecuted;
	int argC;
	char* argV[20];
	char* argv1[20]; //Para el caso que se haga un pipeline.
	char* argv2[20];
	char executePath[256]; //Path del proceso que se va a ejecutar.
	char comando [256]; //Comando que ingresa el usuario.
	
	//Almaceno nombre de usuario logeado y nombre del equipo.
	char hostname [20];
	gethostname(hostname,20);
	//Saco el user name
	pwd = getpwuid(getuid());
	
	//Obtengo todos los paths que están en la variable de entorno PATH
	int pathCounter;
	char *paths[20];
	pathCounter = getPaths(paths);
	
	while (1){
		strcpy(comando, "\n"); //Vacío el comando para que no se ejecute si se presiona Ctrl+D.
		flagWaitPID = 0;	   //Limpio las banderas
		pipeExecuted = 0;

		printf("%s@%s:%s$ ", pwd->pw_name, hostname,getcwd(NULL,50));
		fgets(comando,256,stdin);
		
		//Si comando tiene solo un salto de línea, no hago nada.
		if(!strcmp(comando, "\n")){
			printf("\n");
			continue;
		}
		
		//Esto es lo que se ejecuta si se ingresa algun comando
		else{
			argC = leerTeclado(argV, comando);
			
			//Comandos internos del bash.	
			if(!strcmp(comando,"exit"))
				return 0;
			if(!strcmp(argV[0],"cd")){
				chdir(argV[1]);
				continue;
			}
			
			int flagRedirect = 0;
			int doPipe = 0;
			char fileName[50];
			doPipe = checkPipe(argV, argv1, argv2);
			flagRedirect = checkRedirect(argV, fileName);
			
			//Verifico si el proceso se tiene que ejecutar en Background.
			bgProcess = background(argV);
			if(bgProcess){
				argV[argC-1] = NULL;
				argC--;
			}
			
			buscarArchivo(argV[0], paths, executePath);
			if(executePath[0] == 'X')
				printf("El archivo no fue encontrado\n");
			else{
				pid = fork();
				if (pid<0) {
					perror("Creando el hijo");
					exit(1);
				}
				else if (pid == 0) {	//Proceso hijo.
					if(flagRedirect == 2){
						outPut(fileName);
					}
					else if(flagRedirect == 1){						
						freopen(fileName,"r",stdin);
					}
					else if(doPipe == 1){
						doPipeline(argv1, argv2, paths);
						pipeExecuted = 1;
					}
					if(!pipeExecuted){
						execv(executePath, argV);
						perror(executePath);
						exit(1);
					}
				}
				else{	//Proces padre.
					flagWaitPID = -1;
				}
				if(bgProcess)
					waitpid(pid,&flagWaitPID,WNOHANG);
				else
					waitpid(pid,&flagWaitPID,0);
			}	
		}
	}
	
	return 0;
}

/**
 * Obtiene los Paths de la variable de entorno PATH, y almacena uno por cada elemento del array paths.
 * @param paths Arreglo de punteros a caracter donde se almacena cada path.
 * @return Cantidad de paths encontrados.
 */
int getPaths(char* paths[]){
	int pathCounter;
	char* pathVar = getenv("PATH");
	
	paths[0] = strtok(pathVar, ":");
	for(pathCounter = 1; pathCounter < 20; pathCounter++){
		paths[pathCounter] = strtok(NULL,":");		
		if (paths[pathCounter] == NULL)
			break;
	}
	
	strtok(NULL,":");
	return pathCounter+1;
}

/**
 * Lee la cadena ingresada por el teclado, guarda cada parámetro en el argv[] y devuelve la cantidad de argumentos.
 * @param argv Arreglo de punteros a los argumentos.
 * @param cadena Cadena ingresada por el usuario
 * @return Número de argumentos encontrados en la cadena.
 */
int leerTeclado(char* argv[], char* cadena){
	int wordsCounter = 0;	

	argv[0] = strtok(cadena, " \n");
	for(wordsCounter = 1; wordsCounter < 20; wordsCounter++){
		argv[wordsCounter] = strtok(NULL, " \n");
		if (argv[wordsCounter] == NULL)
			break;
	}
	return wordsCounter;
}

/**
 * Lee la cadena almacenada en arch, busca el ejecutable, y muestra el PATH donde se encuentra
 * @param arch Es el path donde se encuentra el archivo o directorio que se quiere buscar
 * @param paths Arreglo con todos los directorios en los que se quiere buscar
 * @param execPath El path completo al archivo que se encontro. Si no se encuentra el archivo, se pone 'X' en execPath[0]
 */
void buscarArchivo(char* arch, char* paths[], char* execPath){
	char returnPath[50];
	int result;
	char searchDir[50] = "";
	char* archivo;
	strcpy(returnPath, arch);
	
	if(arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/')){
		char* dir;
		char* nextDir;
		int pathReady = 0; //Bandera que controla si ya se obtuvo el path completo
		
		if(arch[0] == '/')
			searchDir[0] = '/';
		dir = strtok(arch,"/");
		nextDir = strtok(NULL,"/");
		
		if(nextDir != NULL) //Si el comando no es de la forma /archivo
			strcat(searchDir,dir);
		else{
			nextDir = dir; //Si archivo si es NULL, entonces pongo en archivo lo que hay despues de la ultima /
			pathReady = 1;
		}

		while((nextDir != NULL) && !pathReady){
			dir = nextDir;
			nextDir = strtok(NULL,"/");
			strcat(searchDir,"/");
			if(nextDir != NULL)
				strcat(searchDir,dir);
		}
		archivo = dir;
	}
	
	else if(arch[0] == '.' && arch[1] == '/'){ //Es un path relativo, tomando como path el directorio actual.
		getcwd(searchDir, 50);
		strcat(searchDir,"/");
		archivo = strtok(arch, "/");
		archivo = strtok(NULL,"/");
	}
	
	else{
		//Tiene que buscar en todos los directorios del path.
		int i;
		char aux[50];
		for(i = 0; i < 20; i++){
			if(paths[i] == NULL)
				break;
			strcpy(aux,paths[i]);
			strcat(aux,"/");
			strcat(aux,arch);
			result = access(aux, F_OK);
			if(!result){
				strcpy(execPath, aux);
				return;
			}
		}
		execPath[0] = 'X';
		return;
	}
	
	strcat(searchDir, archivo);
	result = access(searchDir, F_OK);
	if(!result)
		strcpy(execPath, searchDir);
	else
		execPath[0] = 'X';
}

/**
 * Verifica si se quiere lanzar el proceso en background.
 * @param argv Vector de argumentos que pasa el usuario. Se necesita para verificar si el ultimo es un &.
 * @return Devuelve 0 si el proceso no debe ejecutarse en background, y 1 en caso contrario.
 */
int background(char* argv[]){
	int i;
	for(i = 0; i < 20; i++){
		if(argv[i] == NULL)
			break;
	}
	if(!strcmp(argv[i-1], "&"))
		return 1;
	return 0;
}

/**
 * Verifica si se debe redireccionar la entrada o la salida estandar.
 * @param argv Arreglo que contiene el comando y los argumentos ingresados.
 * @param fileName Almacena el nombre del archivo del que se lee, o en el que se escribe la salida.
 * @return Devuelve 0 si no hay que redireccionar, 1 si hay que redireccionar la entrada, y 2 si hay que redireccionar la salida.
 */
int checkRedirect(char* argv[], char fileName[]){
	int i;
	for (i = 0; i < 20; i++){
			
		if(argv[i] == NULL){
			fileName = NULL;
			return 0;
		}
		else if (!strcmp(argv[i], "<")){
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;		
			argv[i+1] = NULL;
			return 1;
		}
		else if (!strcmp(argv[i], ">")){
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;	
			argv[i+1] = NULL;
			return 2;
		}
	}
	return 0;
}

/**
 * Modifica la salida estándar.
 * @param fileName Path al archivo donde se envía la salida.
 */
void outPut(char fileName[]){
	int fid;
	int flags,perm;
	flags = O_WRONLY|O_CREAT|O_TRUNC;
	perm = S_IWUSR|S_IRUSR;
	
	fid = open(fileName, flags, perm);	
	if (fid<0) {
		perror("open");
		exit(1);
	}
	close(STDOUT_FILENO);
	if (dup(fid)<0) {
		perror("dup");
		exit(1);
	}
	close(fid);
}

/**
 * Modifica la entrada estándar.
 * @param fileName Path al archivo donde se envía la salida.
 */
void inPut(char fileName[]){
	int fid;
	int flags,perm;
	flags = O_RDONLY;
	perm = S_IWUSR|S_IRUSR;
	
	close(STDIN_FILENO);
	fid = open(fileName, flags, perm);	
	if (fid<0) {
		perror("open");
		exit(1);
	}	
	if (dup(fid)<0) {
		perror("dup");
		exit(1);
	}
	close(fid);	
}

/**
 * Verifica si debe realizarse un pipeline.
 * @param argv Argumentos del comando ingresado por el usuario.
 * @param argv1 Array donde se guardarán los argumentos del comando 1.
 * @param argv2 Array donde se guardarán los argumentos del comando 2.
 * @return Devuelve 1 si se debe ejecutar el pipeline. 0 en caso contrario.
 */
int checkPipe(char* argv[], char* argv1[], char* argv2[]){
	int indexArg, aux, indexArg2;
	
	for(indexArg = 0; argv[indexArg] != NULL; indexArg++){
		aux = strcmp("|", argv[indexArg]);
		if(aux == 0)
			break;
		argv1[indexArg] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv1[indexArg], argv[indexArg]);
	}
	argv1[indexArg] = '\0';
	
	//Si no encontró |, se devuelve un 0, si no, se obtienen los dos argv de los comandos.
	if(argv[indexArg] == NULL)
		return 0;
	
	indexArg++;
	
	for(indexArg2 = 0; argv[indexArg] != NULL; indexArg2++){
		
		if(argv[indexArg] == NULL)
			break;
		argv2[indexArg2] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv2[indexArg2], argv[indexArg]);
		indexArg++;
	}
	argv2[indexArg2] = '\0';
	
	return 1;
}

/**
 * Ejecuta el Pipeline
 * @param argv1 Argumentos del comando 1.
 * @param argv2 Argumentos del comando 2.
 * @param paths Paths donde puede buscar los archivos que ejecutan los comandos.
 */
void doPipeline(char* argv1[], char* argv2[], char* paths[]){
	char executePath[256];
	
	int fd[2];
	pipe(fd);
	if (fork()==0) { //codigo del hijo
		close(fd[0]);
		dup2(fd[1],1); // redireccion de la salida al pipe.
		close(fd[1]);
		buscarArchivo(argv1[0], paths, executePath);
		execv(executePath, argv1);
		perror(executePath);
		exit(1);
	} 
	else {
		close(fd[1]); //codigo del padre
		dup2(fd[0],0);
		close(fd[0]);
		buscarArchivo(argv2[0], paths, executePath);
		execv(executePath, argv2);
		perror(executePath);
		exit(1);
	}
}
