set terminal png size 1024,768
set output sprintf("graficas/memoria/MO%d.png", orden)
set title sprintf("Memoria OMP para matrices de %dx%d", orden,orden)
set xlabel "Ejecución"
set ylabel "Memoria (Kb)"
plot sprintf("archivos/memoria/MO%d.txt", orden) smooth unique title "OpenMP", \
     sprintf("archivos/memoria/MOProm%d.txt", orden) with linespoints title "Promedio"
