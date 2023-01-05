set terminal png size 1024,768
set output "graficas/MOProm.png"
set title "Memoria OMP para matrices con long utilizando 2 Cores"
set xrange [0:2048]
set xlabel "Orden de las matrices"
set xtics (64,128,256,512,1024,2048)
set ylabel "Memoria (Kb)"
plot "archivos/memoria/MOProm.txt" smooth unique title "Memoria promedio"
