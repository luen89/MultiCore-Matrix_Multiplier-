set terminal png size 1024,768
set output "graficas/MOProm.png"
set title "Memoria OMP para matrices con float utilizando 4 Cores y 16 hilos"
set xrange [0:2048]
set xlabel "Orden de las matrices"
set xtics (64,128,256,512,1024,2048)
set ylabel "Memoria (Kb)"
plot "archivos/memoria/MOProm.txt" smooth unique title "Memoria promedio"
