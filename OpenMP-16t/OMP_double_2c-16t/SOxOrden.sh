orden=$1
rm archivos/tiempo/TO$orden.txt
rm archivos/tiempo/TOPre$orden.txt
rm archivos/tiempo/TOProm$orden.txt
rm archivos/memoria/MO$orden.txt
rm archivos/memoria/MOPre$orden.txt
rm archivos/memoria/MOProm$orden.txt
g++ -fopenmp -o m mmop.cpp
echo OMP $orden x $orden double 2 cores
for i in {1..10}
do
	echo N: $i
	export OMP_NUM_THREADS=16
	taskset -c 0,1 ./m $orden
done
gcc -o leer leer.c
./leer $orden 10
gnuplot -e "orden=$orden; load 'GraphsTO.p'"
gnuplot -e "orden=$orden; load 'GraphsMO.p'"
