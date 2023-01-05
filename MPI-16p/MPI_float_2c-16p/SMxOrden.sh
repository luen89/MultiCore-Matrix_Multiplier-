orden=$1
rm archivos/tiempo/TM$orden.txt
rm archivos/tiempo/TMPre$orden.txt
rm archivos/tiempo/TMProm$orden.txt
rm archivos/memoria/MM$orden.txt
rm archivos/memoria/MMPre$orden.txt
rm archivos/memoria/MMProm$orden.txt
mpicxx -o m codigo/mmmpi$orden.cpp
echo MPI $orden x $orden float 2 cores 16 procesos
for i in {1..10}
do
	echo N: $i
	taskset -c 0,1 mpirun -np 16 ./m
done
gcc -o leer leer.c
./leer $orden 10
gnuplot -e "orden=$orden; load 'GraphsTM.p'"
gnuplot -e "orden=$orden; load 'GraphsMM.p'"
