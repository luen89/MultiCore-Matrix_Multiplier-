rm archivos/tiempo/TOProm.txt
rm archivos/memoria/MOProm.txt
bash SOxOrden.sh 16
bash SOxOrden.sh 32
bash SOxOrden.sh 64
bash SOxOrden.sh 128
bash SOxOrden.sh 256
bash SOxOrden.sh 512
bash SOxOrden.sh 1024
bash SOxOrden.sh 2048
gnuplot "TOProm.p"
gnuplot "MOProm.p"
