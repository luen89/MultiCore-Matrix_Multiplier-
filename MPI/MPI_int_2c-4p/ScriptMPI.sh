rm archivos/tiempo/TMProm.txt
rm archivos/memoria/MMProm.txt
bash SMxOrden.sh 16
bash SMxOrden.sh 32
bash SMxOrden.sh 64
bash SMxOrden.sh 128
bash SMxOrden.sh 256
bash SMxOrden.sh 512
bash SMxOrden.sh 1024
bash SMxOrden.sh 2048
gnuplot "TMProm.p"
gnuplot "MMProm.p"
