#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

void process_mem_usage(double& vm_usage, double& resident_set){
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

int main (int argc, char *argv[]) {
  if(argc!=2){
    printf("argumentos incorrectos [nombre]_[orden de la matriz]");
    exit(-1);
  }

  double vm, rss;
  int orden=atoi(argv[1]);
  FILE *fT, *fM;
  char nomT[35];
  char nomM[35];
  snprintf(nomT, sizeof(nomT), "archivos/tiempo/TOPre%d.txt",orden);
  snprintf(nomM, sizeof(nomM), "archivos/memoria/MOPre%d.txt",orden);

  fM=fopen(nomM,"a");
  fT=fopen(nomT,"a");

  double t_ini, t_fin;
  double secs;
  time_t t;
  srand((unsigned) time(&t));
  int NRA=orden;
  int NCA=orden;
  int NRC=orden;
  int NCB=orden;
  int	tid, nthreads, i, j, k, chunk;
  sleep(1);
  int** a=(int**) malloc(orden*sizeof(int*));
  for(int l=0;l<orden;l++)
  a[l]=(int*) malloc(orden*sizeof(int));

  int** b=(int**) malloc(orden*sizeof(int*));
  for(int l=0;l<orden;l++)
  b[l]=(int*) malloc(orden*sizeof(int));

  int** c=(int**) malloc(orden*sizeof(int*));
  for(int l=0;l<orden;l++)
  c[l]=(int*) malloc(orden*sizeof(int));


  chunk = 10;  
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= (int)(1000 + rand() % (2000-1000+1));
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= (int)(1000 + rand() % (2000-1000+1));
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;                  /* tamaño de iteraciones del chunk*/

  /***Empieza la seccion paralela y se declaran las variables que se van a usar**/
  t_ini = omp_get_wtime();
  #pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  /*** Initialize se inicializan las matrices ***/
  /*** Se realiza la multiplicacion de matrices compartiendo iteraciones del ciclo externo ***/
  /*** se muestra que proceso hizo la iteracion**/
  //printf("Thread %d starting matrix multiply...\n",tid);

  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)    
    {
    //printf("Thread=%d did row=%d\n",tid,i);
    for(j=0; j<NCB; j++)       
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** Finaliza la seccion parelela ***/
  t_fin = omp_get_wtime();
  process_mem_usage(vm, rss);
  /*** Se imprimen los resultados ***/
  /*printf("******************************************************\n");
  printf("Result Matrix:\n");
  for (i=0; i<NRA; i++)
    {
    for (j=0; j<NCB; j++) 
      printf("%6.2f   ", c[i][j]);
    printf("\n"); 
    }
  printf("******************************************************\n");

  printf ("Done.\n");
  */
  secs = (double)(t_fin - t_ini);
  //printf("%.16g milisegundos\n", secs * 1000.0);
  fprintf(fT,"%f\n",secs );
  fclose(fT);
  fprintf(fM,"%f\n",rss);
  fclose(fM);
  //printf ("Done.\n");
}