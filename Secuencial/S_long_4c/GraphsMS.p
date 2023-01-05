set terminal png size 1024,768
set output sprintf("graficas/memoria/MS%d.png", orden)
set title sprintf("Memoria secuencial para matrices de %dx%d", orden,orden)
set xlabel "Ejecución"
set ylabel "Memoria (Kb)"
plot sprintf("archivos/memoria/MS%d.txt", orden) smooth unique title "Secuencial", \
     sprintf("archivos/memoria/MSProm%d.txt", orden) with linespoints title "Promedio"
