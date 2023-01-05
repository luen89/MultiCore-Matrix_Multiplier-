Se recomiendo antes de usar un archivo del tipo Script.sh ejecutar el comando chmod +x Script.sh

Para instalar las bibliotecas necesarias basta con ejecutar el script instalar.sh ademas se puede usar el script instalar_ejecutar.sh que ademas ejecuta todas las variantes de los algoritmos


Para ejecutar los algoritmos de multiplicacion de matrices proponemos 5 maneras diferentes de realizarlo:

	1 EJECUTAR TODAS LAS VARIANTES
		1.1-ejecutar el script ejecutar.sh el cual ejecutara todas las varientes del algoritmos con diferentes numeros de cores activos, diferente numero de procesos, diferentes ordenes de matriz, diferente tipo de dato y plataformas

	2 EJECUCION ESPECIFICANDO PLATAFORMA
		2.1-entrar a la carpeta que requieramos segun la plataforma: 
			2.1.1 MPI o MPI-16t(Exclusivo de 16 procesos en ejecucion)  ejecutar el script MPI.sh que nos hara todas las corridas de MPI con variantes en el numero de cores, en el numero de procesos y orden de matriz

			2.1.2 OpenMp o OpenMP-16p (Exclusivo de 16 procesos en ejecucion)  ejecutar el script OpenMp.sh que nos hara todas las corridas de OpenMP con variantes en el numero de cores, en el numero de procesos y orden de matriz

			2.1.3 SecuenciaL y ejecutar el script Secuencial.sh que nos hara todas las corridas de MPI con variantes en el numero de cores, en el numero de procesos y orden de matriz

	3 EJECUCION ESPECIFICANDO PLATAFORMA Y VARIENTE EN NUMERO DE CORES Y PROCESOS
		3.1 entrar a la carpeta que requieramos segun la plataforma: 
			3.1.1 MPI o MPI-16t(Exclusivo de 16 procesos en ejecucion)
				3.1.2 Entrar en el alguna de las carpetas cuyo formato es MPI_[tipo_de_dato]_[numero de cores]_[numero de procesos] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script ScriptMPI.sh el cual hara 10 ejecuciones para cada orden de matriz con la variante que hayamos seleccionado.

			3.1.2 OpenMp o OpenMP-16p (Exclusivo de 16 procesos en ejecucion)
				3.1.2.1 Entrar en el alguna de las carpetas cuyo formato es OMP_[tipo_de_dato]_[numero de cores]_[numero de procesos] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script ScriptOMP.sh el cual hara 10 ejecuciones para cada orden de matriz con la variante que hayamos seleccionado.

			3.1.3 Secuencial
				3.1.3.1 Entrar en el alguna de las carpetas cuyo formato es S_[tipo_de_dato]_[numero de cores] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script ScriptOMP.sh el cual hara 10 ejecuciones para cada orden de matriz con la variante que hayamos seleccionado.

ADVERTENCIA: No recomendamos ejecutar de las siguientes maneras sin embargo las incluimos en que caso de que se pueda requerir pero ha de saberse que aunque los resultados de tiempo si seran registrados en los archivos de tiempo no seran considerados en los promedios y por consiguiente en algunos casos en las graficas

	4 DIEZ EJECUCIONES DE UN ORDEN DE MATRIZ
		4.1 entrar a la carpeta que requieramos segun la plataforma: 
			4.1.1 MPI o MPI-16t(Exclusivo de 16 procesos en ejecucion)
				4.1.1.1 Entrar en el alguna de las carpetas cuyo formato es MPI_[tipo_de_dato]_[numero de cores]_[numero de procesos] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script SMxOrden.sh de la siguiente manera [SMxOrden.sh orden_de_matriz] (procure que el orden tanga los valores de 16,32,64,128,256,512,1024,2048 de lo contrario obtendra un error). este script hara 10 ejecuciones del orden de matriz que se haya mandado con la variante que hayamos seleccionado.

			4.1.2 OpenMp o OpenMP-16p (Exclusivo de 16 procesos en ejecucion)
				4.1.2.1 Entrar en el alguna de las carpetas cuyo formato es OMP_[tipo_de_dato]_[numero de cores]_[numero de procesos] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script SOxOrden.sh de la siguiente manera [SOxOrden.sh orden_de_matriz]. este script hara 10 ejecuciones del orden de matriz que se haya mandado con la variante que hayamos seleccionado.


			4.1.3 Secuencial
				4.1.3.1 Entrar en el alguna de las carpetas cuyo formato es S_[tipo_de_dato]_[numero de cores] segun la variante que quieramos ejecutar. Y Finalmente ejecutar el script SSxOrden.sh de la siguiente manera [SSxOrden.sh orden_de_matriz]. este script hara 10 ejecuciones del orden de matriz que se haya mandado con la variante que hayamos seleccionado.

	5 UNA UNICA EJECUCION DONDE EL USUARIO INDICA TODOS LOS PARAMETROS	

		5.1-entrar a la carpeta que requieramos segun la plataforma: 
			5.1.1 MPI o MPI-16t(Exclusivo de 16 procesos en ejecucion)
				5.1.1.1 Entrar en el cualquiera de las carpetas cuyo formato es MPI_[tipo_de_dato]_[numero de cores]_[numero de procesos]  y entrar al directorio codigos, el formato de cada codigo es mmmpi_orden de matriz seleccionar el orden de matriz que deseamos compilar el codigo con  mpicxx -o m codigo/mmmpi_orden.cpp y ejecutarlo con taskset -c [cores_a_utilizar] mpirun -np [numero_de_procesos] ./m esto hara una sola ejecucion del algoritmo con los parametros que haya seleccionado

			5.1.2 OpenMp o OpenMP-16p (Exclusivo de 16 procesos en ejecucion)
				5.1.2.1 Entrar en cualquiera de las carpetas cuyo formato es OMP_[tipo_de_dato]_[numero de cores]_[numero de procesos]. Y compilar el archivo mmop.cpp de la siguiente manera g++ -fopenmp -o m mmop.cpp y de la siguiente manera [taskset -c [Cores] ./m $orden]. esto hara una ejecucion del orden de matriz que se haya mandado con la variante que hayamos seleccionado.

			5.1.3 Secuencial
				5.1.3.1 Entrar en el alguna de las carpetas cuyo formato es S_[tipo_de_dato]_[numero de cores]. compilar el archivo  mms.cpp de la siguiente manera g++ -o m mms.cpp despues  ejecutarlo de la siguiente forma [taskset -c [Cores] ./m (orden)]. este script hara 1 ejecucion del orden de matriz que se haya mandado con la variante que hayamos seleccionado.	



