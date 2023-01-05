orden=$1
rm archivos/tiempo/TS$orden.txt
rm archivos/tiempo/TSPre$orden.txt
rm archivos/tiempo/TSProm$orden.txt
rm archivos/memoria/MS$orden.txt
rm archivos/memoria/MSPre$orden.txt
rm archivos/memoria/MSProm$orden.txt
g++ -o m mms.cpp
echo Secuencial $orden x $orden Floats 2 cores
for i in {1..10}
do
	echo N: $i
	taskset -c 0,1 ./m $orden
done
gcc -o leer leer.c
./leer $orden 10
gnuplot -e "orden=$orden; load 'GraphsTS.p'"
gnuplot -e "orden=$orden; load 'GraphsMS.p'"
