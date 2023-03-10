set terminal png size 1024,768
set output "graficas/OSProm.png"
set title "Tiempo OMP para matrices con long utilizando 4 Cores y 16 hilos"
set xrange [0:2048]
set xlabel "Orden de las matrices"
set xtics (64,128,256,512,1024,2048)
set ylabel "Tiempo (s)"
plot "archivos/tiempo/TOProm.txt" smooth unique title "Tiempo promedio"
