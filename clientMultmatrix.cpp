#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "multmatrix_stub.h"

void freeMatrix(matrix_t* m){
    delete[] m->data;
    delete[] m;
}

int main(int argc, char **argv)
{
    // crear objeto
	multMatrix* mmatrix=new multMatrix();

    // operaciones
    matrix_t* m1= mmatrix->createRandMatrix(5,5);
    matrix_t* m2= mmatrix->createIdentity(5,5);
    matrix_t* mres=mmatrix->multMatrices(m1,m2);
    mmatrix->writeMatrix(mres,"resultado.txt");
    matrix_t* m3=mmatrix->readMatrix("resultado.txt");
    matrix_t* mres2=mmatrix->multMatrices(m1,m3);
    mmatrix->writeMatrix(mres2,"resultado2.txt");


    // liberar memoria
    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(m3);
    freeMatrix(mres2);
    delete mmatrix;

    return 0;
}
