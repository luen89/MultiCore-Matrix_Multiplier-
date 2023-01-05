#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main ( int argc, char **argv ){
    FILE *fPreTime, *fTime, *fTPromA, *fTPromP, *fPreMem, *fMem, *fMPromP, *fMPromA;
    if(argc!=3){
		printf("argumentos incorrectos [nombre]_[orden de la matriz]_[n Pruebas]\n");
		exit(-1);
	}
    int rango=atoi(argv[1]);
    int nPruebas=atoi(argv[2]);
 	//int rango=16, nPruebas=10;
 	char buffer[100], snum[4];
 	double pT[nPruebas], pM[nPruebas];
    double aux=0.0, promedioT=0.0, promedioM=0.0;

    char time[35];
    snprintf(time, sizeof(time), "archivos/tiempo/TM%d.txt",rango);
    char preTime[35];
    snprintf(preTime, sizeof(preTime), "archivos/tiempo/TMPre%d.txt",rango);
    char promTP[35];
    snprintf(promTP, sizeof(promTP), "archivos/tiempo/TMProm%d.txt",rango);

    char mem[35];
    snprintf(mem, sizeof(mem), "archivos/memoria/MM%d.txt",rango);
    char preMem[35];
    snprintf(preMem, sizeof(preMem), "archivos/memoria/MMPre%d.txt",rango);
    char promMP[35];
    snprintf(promMP, sizeof(promMP), "archivos/memoria/MMProm%d.txt",rango);

 	fPreTime = fopen ( preTime, "r" );
    fTime = fopen ( time, "a" );
    fTPromA = fopen ( "archivos/tiempo/TMProm.txt", "a" );
    fTPromP = fopen ( promTP, "a");

    fPreMem = fopen ( preMem, "r" );
    fMem= fopen ( mem, "a" );
    fMPromA = fopen ( "archivos/memoria/MMProm.txt", "a" );
    fMPromP = fopen ( promMP, "a");

    int i=1;
 	while(i<=nPruebas){
        fscanf(fPreTime, "%s" ,buffer);
        sscanf(buffer, "%lf", &aux);
        pT[i-1]=aux;
        fprintf(fTime,"%d\t\t%s\n",i,buffer);

        fscanf(fPreMem, "%s" ,buffer);
        sscanf(buffer, "%lf", &aux);
        pM[i-1]=aux;
        fprintf(fMem,"%d\t\t%s\n",i,buffer);

        i++;
    }    

    for(i=0; i<nPruebas; i++){
        promedioT+=pT[i];
        promedioM+=pM[i];
    }
    promedioT=promedioT/nPruebas;
    promedioM=promedioM/nPruebas;

    fprintf(fTPromP,"%d\t\t%lf\n",1,promedioT);
    fprintf(fTPromP,"%d\t\t%lf\n",nPruebas,promedioT);
    fprintf(fTPromA,"%d\t\t%lf\n",rango,promedioT);

    fprintf(fMPromP,"%d\t\t%lf\n",1,promedioM);
    fprintf(fMPromP,"%d\t\t%lf\n",nPruebas,promedioM);
    fprintf(fMPromA,"%d\t\t%lf\n",rango,promedioM);

    fclose ( fPreTime );
    fclose ( fTime );
    fclose ( fTPromA );
    fclose ( fTPromP );
 	
 	fclose ( fPreMem );
 	fclose ( fMem );
    fclose ( fMPromA );
    fclose ( fMPromP );
 	return 0;
}
