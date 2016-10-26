# Trabajo practico 2
## Sistemas operativos 1
### Baash
---
#### Alumno : Fabian Wolfmann
##### Matricula: 39073116
----
### Objetivos
    -Utilizar mecanismos de concurrencia y comunicación UNIX
    -Implementar de manera sencilla un intérprete de línea de comandos (Shell) al estilo de Bourne Shell.
    -Utilizar compilador gcc y archivos makefile
---
### Introduccion
En el siguiente trabajo, se mostrara la elaboracion de un Shell que interprete comandos de Linux utilizando la misma sintaxis de Bourne Shell. Los comandos serán introducidos por el usuario e interpretados por el Shell.

Para lograr la comprensión del funcionamiento del Bourne Shell (utilizado por la mayoría de distribuciones de Linux), se nos presentan una serie de ejercicios que se responden al comienzo de la etapa de desarrollo. Luego se realiza la programación de nuestro propio Baash.

### Desarrollo
####Ejercicios Parte I

1. ¿Cómo ejecutamos un archivo que se llama exactamente como un builtin?
	* La manera de ejecutar un archivo que se llama exactamente como un builtin es utilizando el camino absoluto o relativo al archivo, ya que si queremos ejecutarlo con la búsqueda en secuencia en la variable de entorno PATH, primero se buscará en los comandos internos implementados en Baash.

2. ¿Por qué las recomendaciones de seguridad indican que es peligroso tener ./ en PATH al más puro estilo DOS?
	* Si se agrega './' al PATH, no será necesario anteponer ./ antes de ejecutar un archivo que se encuentre en el directorio actual, por lo tanto es inseguro tener ./ en el PATH ya que podría ejecutarse cualquier archivo sin anteponer el ./ antes del nombre del ejecutable.

3. Supongamos que existen 2 comandos posibles dentro de la secuencia que contiene PATH, donde el primero en la secuencia no está marcado como ejecutable y el segundo si, ¿Qué hace el intérprete Baash? ¿ejecuta el segundo o informa que el primero no tiene permiso de ejecución?
	* El intérprete Baash informa que el primero no tiene permiso de ejecución.

4. Indique qué sucede cuando se tiene un directorio en el pwd actual con el mismo nombre que un archivo ejecutable en el PATH. ¿Dice que no puede ejecutar un directorio o ejecuta el comando que encontró?
	* Ejecuta el comando que encontró. Por ejemplo, si en el pwd actual creamos un directorio con el nombre sleep, al ejecutar el comando 'sleep' se ejecuta el comando que se encuentra al recorrer la secuencia de PATH. Para que aparezca el error de que no puede ejecutarse un directorio, el comando debe ser './sleep'.

5. Obtengan la lógica más sencilla que unifique los tres tipos de búsqueda.
	* *SI* `(argumv [posicion][0] == '.' || argumv [posicion][0] == '/')` -> Crear hijo y ejecutar archivo 
	*SINO* -> Buscar en PATH y si encuentra archivo crear hijo y ejecutarlo.

6. ¿Podemos poner directorios relativos en PATH?
	* En PATH pueden agregarse directorios relativos.

#### Ejercicios Parte II

7. Investiguen cuales son los comandos internos para manejo de procesos en background de Baash.
	* Algunos de los comandos que se utilizan para ejecutar procesos en segundo plano son los siguientes:
`<comando>` `&` Ejecuta el proceso en segundo plano.
*bg* [PID] Lleva el proceso PID a background.
*fg* [PID] Lleva el proceso PID a foreground. 
En estos dos casos, el PID es el que se muestra con el comando bg o fg.

8. En el ejemplo de arriba el operador `&` funciona como operador de composición paralela. Hay un operador de composición secuencial en Bourne Shell. ¿Cuál es?
	* El operador de composición secuencial es `;` (punto y coma).

9. Investiguen como Baash forma el árbol de procesos cuando ejecutamos cmd1 & cmd2 & cmd3 & ... & cmdN. Piensen la respuesta y luego utilicen pstree para corroborarla. Apliquen los resultados de su observación a Baash.
	* Cuando se ejecutan, lo hacen en paralelo con la siguiente lógica: 
El proceso de arranque **init** en *GNU/Linux* es el primer proceso que se ejecuta en el sistema, además de ser el más importante, y de quien dependen el resto de los procesos. Es el kernel quien cede el control al init. A partir de init donde se crean los primeros procesos hijos, y a su vez estos ejecutan otros procesos, obteniéndose un árbol de procesos.
Init, lee el archivo **/etc/ttytab** para ver cuantas terminales hay, y mediante **fork** crea un proceso hijo para cada terminal. Cada proceso hijo ejecuta **getty**, que es quien imprime el prompt del login y obtiene el nombre de usuario. Cuando el usuario entra al sistema, se ejecuta **login** con el nombre como argumento y se ejecuta el Shell de ese usuario. El shell del usuario se especifica en el **/etc/passwd**, generalmente **sh**. El Shell espera por un comando y luego lanza un FORK y un EXEC por cada comando.
Es así, que el shell es hijo del init, los procesos de usuario los nietos, y todos los procesos forman parte de un mismo árbol.
(fuente: [http://luauf.com/2008/06/21/arbol-de-procesos-en-linux/])

10. Indique cuantas letras 'a' debería imprimir el siguiente programa: 
 * fork();
 * fork();
 * fork();
 * printf("a");
	a	a	a
	a	a	a
		a	a
		a	a
			a
			a
			a
			a
	2	4	8
			
Al final se imprimiran 8 `a`

#### Ejercicios Parte III


11. Las corrientes estándar stdout y stderr están dirigidas ambas a la consola. ¿Cómo podemos utilizar los operadores de redirección para separarlas?
	* En forma genérica se puede lograr de esta forma: (comando) > archivo.
	Un ejemplo:
	cat hola.txt > chau.txt lo que hace es redireccionar al archivo chau.txt

12. De más ejemplos de cómo usar la redirección de entrada. Tenga cuidado que muchos programas deshabilitan la posibilidad de redirigir la entrada estándar a otra cosa que no sea una terminal.
	* Otro ejemplo es el siguiente (en forma genérica):  `(comando) < archivo`
	Lo que hace esto es imprimir en pantalla (Baash) lo que haga el comando con el archivo.
	Ejemplo: `head -5 < file.txt`
	
#### Ejercicios Parte IV
13. Los pipes existen en el filesystem mientras dura el comando. ¿Dónde se encuentran y que atributos tiene?
	* Las tuberías se emplean para interconectar procesos, es decir, podemos realizar varios procesos con una sola línea de comando. Su uso más habitual es pasar el resultado de la ejecución de un comando a la entrada de otro. 

