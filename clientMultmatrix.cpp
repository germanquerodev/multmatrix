#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "multmatrix_stub.h"

int main(int argc, char **argv)
{

	multMatrix p = multMatrix();
	matrix_t *mi = p.createIdentity(2, 2);
	matrix_t *mr = p.createRandMatrix(2, 2);
	matrix_t *mm = p.multMatrices(mi, mr);
	p.writeMatrix(mi,"mi.txt");
	p.writeMatrix(mr,"mr.txt");
	p.writeMatrix(mm,"mm.txt");
	matrix_t *maux = p.readMatrix("mi.txt");
	p.writeMatrix(maux,"maux.txt");
	return 0;
}
