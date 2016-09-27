# Trabajo practico 1
## Sistemas operativos 1
### Datos en /proc
---
#### Alumno : Fabian Wolfmann
##### Matricula: 39073116
----
### Objetivos
    -Familiarizarse con la programacion en C para sistemas operativos
    -Tener contacto con el sistema operativo Linux, en sus versiones, en este caso Ubuntu 
    -Utilizar compilador gcc y archivos makefile
---
### Introduccion
En los sistemas operativos de licencia abierta, basados en Linux, tienen su codigo libre, uno puede modificar los datos como el kernel o cualquier cosa. Eso es un beneficio pero se tiene que tener cuidado a la hora de manipular eso.

Los datos de la computadora y del sistema operativo se almacenan en la carpeta */proc* el programa desarrollado para la busqueda de estos datos, parsearlos e imprimirlos se hace mediante el programa ksamp que acontinuacion se explicara.

### Desarrollo
#### Parte A
Esta parte es el requisto completo del sistema a crear, luego se desglosa en Parte B, C y D

    -Tipo y modelo de CPU 					*/proc/cpuinfo*
	-Versión actual del kernel 				*/proc/version_signature*
	-Tiempo desde que se inició el sistema	*/proc/uptime*
	-Tiempo de CPU ha sido empleado 		*/proc/stat*
	-Memoria total y disponible 			*/proc/meminfo*
	-Pedidos de lectura/escritura a disco 	*/proc/diskstats*
	-Cambios de contexto realizados 		*/proc/stat*
	-Procesos creados desde inicio del SO 	*/proc/stat*
	
Luego en cada parte seran implementadas.

#### Parte B
En esta version inicial de ksamp se imprimira la siguiente informacion:

    -Tipo y modelo de CPU: en */proc/cpuinfo*
        -vendor_id: Tipo de CPU
        -model name: Modelo de CPU
	-Versión actual del kernel 				*/proc/version_signature*
	-Cantidad de tiempo que transcurrio desde que se inicio el sitema operativo en */proc/uptime*
	-Cabecera que indique el nombre de la máquina, fecha y hora actuales.
![](/screens/parteB)

#### Parte C
En esta version del ksamp se utilizan las mismas impreciones que la version anterior pero en el caso de poner la opcion (-S) se agregara la siguiente informacion:

    -Cantidad de tiempo de CPU utilizado para usuarios, sistema y proceso idle.
    -Cantidad de cambios de contexto.
    -Fecha y hora cuando el sistema fue iniciado.
    -Número de procesos creados desde el inicio del sistema
    
![](/screens/parteC)

#### Parte D
Esta version final imprimira la parte B y si le agregamos como parametro -l [ intervalo de tiempo]  se imprimen ademas: 

    -Número de peticiones a disco realizadas.
    -Cantidad de memoria configurada en el hardware.
    -Cantidad de memoria disponible.
    -Lista de los promedios de carga de 1 minuto.
    
![](/screens/parteD)

### Conclusion
Con la ejecucion de este practico se pudo aprender a tomar archivos de texto desde una carpeta de archivos, comprension de los archivos makefile, adaptacion al lenguaje C para sistemas operativos, y la mejor familiarizacion con Linux



