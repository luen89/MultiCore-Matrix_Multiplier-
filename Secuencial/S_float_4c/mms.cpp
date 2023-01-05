#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>


void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

int main (int argc, char *argv[]){
	if(argc!=2){
		printf("argumentos incorrectos [nombre]_[orden de la matriz]");
		exit(-1);
	}
	double vm, rss;
   	

	int orden=atoi(argv[1]);
	FILE *fT, *fM;
	char nomM[35];
	char nomT[35];
	snprintf(nomT, sizeof(nomT), "archivos/tiempo/TSPre%d.txt",orden);
	snprintf(nomM, sizeof(nomM), "archivos/memoria/MSPre%d.txt",orden);

	fM=fopen(nomM,"a");
	fT=fopen(nomT,"a");
	//if (fp==NULL) {fputs ("File error",stderr); exit (1);}
	clock_t t_ini, t_fin;
	double secs;
	int	i, j, k;
	time_t t;
	srand((unsigned) time(&t));

	float** a=(float**) malloc(orden*sizeof(float*));
	for(int l=0;l<orden;l++)
	a[l]=(float*) malloc(orden*sizeof(float));

	float** b=(float**) malloc(orden*sizeof(float*));
	for(int l=0;l<orden;l++)
	b[l]=(float*) malloc(orden*sizeof(float));

	float** c=(float**) malloc(orden*sizeof(float*));
	for(int l=0;l<orden;l++)
	c[l]=(float*) malloc(orden*sizeof(float));
	  /*** Initialize se inicializan las matrices ***/
	sleep(1);
	  for (i=0; i<orden; i++)
	    for (j=0; j<orden; j++)
	      a[i][j]= (float)(1000 + rand() % (2000-1000+1));
	  for (i=0; i<orden; i++)
	    for (j=0; j<orden; j++)
	      b[i][j]= (float)(1000 + rand() % (2000-1000+1));
	  for (i=0; i<orden; i++)
	    for (j=0; j<orden; j++)
	      c[i][j]= 0;

	  /*** Se realiza la multiplicacion de matrices compartiendo iteraciones del ciclo externo ***/
	  /*** se muestra que proceso hizo la iteracion**/	  	
	  	t_ini = clock();	  	
	  	for (i=0; i<orden; i++)
	    	for(j=0; j<orden; j++)
	      		for (k=0; k<orden; k++)
	        		c[i][j] += a[i][k] * b[k][j];
	     /*** Finaliza la seccion parelela ***/	    
	    t_fin = clock();
	    process_mem_usage(vm, rss);
   		//printf ("VM: %f, RSS: %f\n", vm, rss);
	    /*** Se imprimen los resultados ***/
	    /*printf("******************************************************\n");
	    printf("Result Matrix:\n");
	    for (i=0; i<orden; i++)
	    {
	    for (j=0; j<orden; j++)
	        printf("%6.2f   ", c[i][j]);
	    printf("\n");
	    }
	    printf("******************************************************\n");*/
	    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	    //printf("%.16g milisegundos\n", secs * 1000.0);
	    fprintf(fT,"%f\n",secs );
	    fclose(fT);
	    fprintf(fM,"%f\n",rss);
	    fclose(fM);
	    //printf ("Done.\n");
}
