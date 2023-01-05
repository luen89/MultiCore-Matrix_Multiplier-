set terminal png size 1024,768
set output sprintf("graficas/memoria/MM%d.png", orden)
set title sprintf("Memoria MPI para matrices de %dx%d", orden,orden)
set xlabel "Ejecución"
set ylabel "Memoria (Kb)"
plot sprintf("archivos/memoria/MM%d.txt", orden) smooth unique title "MPI", \
     sprintf("archivos/memoria/MMProm%d.txt", orden) with linespoints title "Promedio"
