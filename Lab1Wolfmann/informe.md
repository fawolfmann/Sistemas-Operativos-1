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
Se pide:
    -Tipo y modelo de CPU 					*/proc/cpuinfo*
	-Versión actual del kernel 				*/proc/version_signature*
	-Tiempo desde que se inició el sistema	*/proc/uptime*
	-Tiempo de CPU ha sido empleado 		*/proc/stat*
	-Memoria total y disponible 			*/proc/meminfo*
	-Pedidos de lectura/escritura a disco 	*/proc/diskstats*
	-Cambios de contexto realizados 		*/proc/stat*
	-Procesos creados desde inicio del SO 	*/proc/stat*
![](/screens/parteB)
