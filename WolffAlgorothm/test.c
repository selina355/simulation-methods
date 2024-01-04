#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "array_alloc.h"
void swap(int**M , int a, int b){
    int *temp= M[a];
    M[a]=M[b];
    M[b]=temp;
}

int main(){
    int n=5;
    int i=0;
    int** matrix=imatrix(0,1,0,n-1);
    for (i=0;i<n;i++){
        matrix[0][i]=0;
    }
    for (i=0;i<n;i++){
        matrix[1][i]=1;

    }

    printf("%d",matrix[0][1]);




    swap(matrix,0,1);
    printf("%d",matrix[0][1]);
    free_imatrix(matrix);

    
}