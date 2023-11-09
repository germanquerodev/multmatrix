#ifndef MULTMATRIX_H
#define MULTMATRIX_H

// estructura de matrix
typedef struct matrix_t
{
    int rows;
    int cols;
    int* data;
}matrix_t;

// clase distribuida
class multMatrix
{
public:
    multMatrix();
    ~multMatrix();
    matrix_t* readMatrix(const char* fileName);
    matrix_t *multMatrices(matrix_t* m1, matrix_t *m2);
    void writeMatrix(matrix_t* m, const char *fileName);
    matrix_t *createIdentity(int rows, int cols);
    matrix_t *createRandMatrix(int rows, int cols);
};

#endif
