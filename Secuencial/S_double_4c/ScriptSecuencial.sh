rm archivos/tiempo/TSProm.txt
rm archivos/memoria/MSProm.txt
bash SSxOrden.sh 16
bash SSxOrden.sh 32
bash SSxOrden.sh 64
bash SSxOrden.sh 128
bash SSxOrden.sh 256
bash SSxOrden.sh 512
bash SSxOrden.sh 1024
bash SSxOrden.sh 2048
gnuplot "TSProm.p"
gnuplot "MSProm.p"
