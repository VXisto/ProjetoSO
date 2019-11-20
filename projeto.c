#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define TAMANHOMAX 100


//a estrutura que passa os valores pras threads
typedef struct valores{
    int i;
    int f;
	int size;
}v;

int *n;

//ordenar os blocos no final
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */	
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
	}

//esta funcao recebe uma estrutura que eh passada para para o pthread criar e chamar o algoritimo de sort para ordenacao
void create(v stc){
    mergeSort(n, stc.i, stc.f);
}

int main(int argc, char *argv[]) {
    int count=0, tv=1,nt=0,nvt,aux=0;
    FILE * fp,*fo;
    register int x=0,y=2;

    //aqui sao recebidas as parametrizacoes de threads
    nt= atoi(argv[1]);

    //aqui eh alocado dinamicamente o valor de estrutura
    v* v= malloc(nt*sizeof(v));

    //error-checking
    if((nt>16) || ((nt%2)>0) || (nt==10) || (nt==6) || (nt==12) || (nt==14)){
        printf("Esta errado! No programa somente use 1,2,4,8 ou 16 threads");
        exit(0);

    }
    if (argc<2) {
        printf("Erro! Faca a chamada desta forma: %s numero_de_threads arq1.in arqn... arquivosaida\n",argv[0]);
        exit(0);
  }

  //aqui eh feito a leitura dos arquivos de entrada
  n= calloc(TAMANHOMAX,sizeof(int));
  for(;y<=argc-2;y++){
    fp = fopen(argv[y],"r");
    for(;!feof(fp);x++){
      if(x<TAMANHOMAX*tv){
       fscanf(fp,"%d",&n[x]);
      }else{
        tv++;
        n=realloc(n,TAMANHOMAX*tv*sizeof(int));
        fscanf(fp,"%d",&n[x]);
      }
    }
  }
  n= realloc(n,(x+1)*sizeof(int));

  nvt=x-1;

    //aqui eh criado os ids das threads
    pthread_t id[nt];

    //aqui os valores dos vetores sao divididos para as threads
    int cont=0;
        if(nt==1){
        v[0].i=0;
        v[0].f=nvt-1;
        }else{
        int nvtd = nvt/nt ;
        float aux2 = nvt/nvtd;
        v[0].i=0;
        v[0].f= nvtd;
        cont++;
        for(;cont<nt;cont++){
            if(cont==nt-1){
                v[cont].i = ((v[cont-1].f)+1);
                v[cont].f = nvt-1;

            }else
            v[cont].i= ((v[cont-1].f)+1);
            v[cont].f= ((v[cont].i)+(nvtd));

            }
        }
     clock_t begin = clock();

    //aqui as threads sao criadas
    int rc;
    int cont2=0;
    for(cont2=0;cont2<nt;cont2++){
        if ((rc = pthread_create(&id[cont2], NULL, create, &v[cont2]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
        }
    }

    //Aqui as threads sao juntadas
    int cont1 = 0;
    for (cont1 = 0; cont1 < nt; ++cont1) {
    pthread_join(id[cont1], NULL);
  }

   //inicio do mergesort para ordenar as threads
	if(nt == 2){
		merge(n,v[0].i,v[1].i,v[1].f);
	} else {
		mergeSort(n,0,nvt-1);
	}
    clock_t end = clock();
   double time_spent = (double)(end-begin)/ CLOCKS_PER_SEC;
  //aqui o arquivo saida.dat eh aberto
  fo=fopen(argv[argc-1],"w");



    //aqui escreve o vetor no arquivo
  for (y=0; y<nvt; y++) {
    fprintf(fo, "%d\n", n[y]);
  }




    //aqui o arquivo eh fechado
        fclose(fo);
    printf("O tempo total das threads eh: %f\n\n",time_spent);
   printf("---------------------------------------------------------\n");
   printf(" O arquivo '%s' foi criado com sucesso!!!                      \n", argv[argc-1]);
   printf("---------------------------------------------------------\n");

  return 0;
}
