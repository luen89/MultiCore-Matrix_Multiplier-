set terminal png size 1024,768
set output sprintf("graficas/tiempo/TS%d.png", orden)
set title sprintf("Memoria secuencial para matrices de %dx%d", orden,orden)
set xlabel "Ejecución"
set ylabel "Tiempo (s)"
plot sprintf("archivos/tiempo/TS%d.txt", orden) smooth unique title "Secuencial", \
     sprintf("archivos/tiempo/TSProm%d.txt", orden) with linespoints title "Promedio"
