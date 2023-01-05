set terminal png size 1024,768
set output sprintf("graficas/tiempo/TM%d.png", orden)
set title sprintf("Memoria MPI para matrices de %dx%d", orden,orden)
set xlabel "Ejecución"
set ylabel "Tiempo (s)"
plot sprintf("archivos/tiempo/TM%d.txt", orden) smooth unique title "MPI", \
     sprintf("archivos/tiempo/TMProm%d.txt", orden) with linespoints title "Promedio"
