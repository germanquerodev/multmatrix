#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"


void freeMatrix(matrix_t* m){
    delete[] m->data;
    delete[] m;
}

// clase interfaz cliente
class multMatrix_imp
{
private:
	int clientId = -1;
	multMatrix *p = nullptr;

public:
	multMatrix_imp(int clientId) : clientId(clientId){};

	bool connectionClosed()
	{
		return p == nullptr;
	}

	void recibeOp()
	{
		// inicializa rpcs
		std::vector<unsigned char> rpcIn;
		std::vector<unsigned char> rpcOut;

		// recibe el paquete
		recvMSG(clientId, rpcIn);

		// desempaqueta la operacion
		matrixOp operacion = unpack<matrixOp>(rpcIn);

		// ejecuta dependiendo de la operacion
		switch (operacion)
		{
		case constructorOp:
		{

			// inicializa el objeto
			p = new multMatrix();
			// devulelve ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case destructorOp:
		{

			// libera la memoria del objeto
			delete p;
			p = nullptr;
			// devuelve el ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case readMatrixOp:
		{
			// desempaqueta la dimension del string
			int s = unpack<int>(rpcIn);
			// reserva memoria y desempaqeta el string del nombre del fichero
			std::string fileName;
			fileName.resize(s);
			unpackv(rpcIn, (char *)fileName.data(), s);

			// declara un objeto matriz y guarda el resultado de readMatrix() dentro
			matrix_t *m = p->readMatrix((char *)fileName.data());

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);

			// empaqueta las filas y columnas
			pack(rpcOut, m->rows);
			pack(rpcOut, m->cols);

			// desempaqueta la matriz
			s = m->rows * m->cols;
			packv(rpcOut, m->data, s);

			// libera memoria
			freeMatrix(m);
		}
		break;
		case multMatricesOp:
		{
			// M1
			// declara una nueva matriz
			matrix_t *m1 = new matrix_t[1];
			// desempaqueta las filas y las columnas
			m1->rows = unpack<int>(rpcIn);
			m1->cols = unpack<int>(rpcIn);
			// reserva memoria y desempaqueta la matriz
			int size = m1->rows * m1->cols;
			m1->data = new int[size];
			unpackv(rpcIn, m1->data, size);

			// M2
			// declara una nueva matriz
			matrix_t *m2 = new matrix_t[1];
			// desempaqueta las filas y las columnas
			m2->rows = unpack<int>(rpcIn);
			m2->cols = unpack<int>(rpcIn);
			// reserva memoria y desempaqueta la matriz
			size = m2->rows * m2->cols;
			m2->data = new int[size];
			unpackv(rpcIn, m2->data, size);

			// declara una nueva matriz y la iguala al resultado de multMatrices()
			matrix_t *mres = p->multMatrices(m1, m2);

			// libera la memoria de m1 y m2
			freeMatrix(m1);
			freeMatrix(m2);

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);

			// empaqueta las filas y columnas
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);
			// empaqueta la matriz
			size = mres->rows * mres->cols;
			packv(rpcOut, mres->data, size);

			// libera memoria
			freeMatrix(mres);
		}
		break;

		case writeMatrixOp:
		{
			// declara una nueva matriz
			matrix_t *m = new matrix_t[1];

			// desempaqueta las filas y columnas
			m->rows = unpack<int>(rpcIn);
			m->cols = unpack<int>(rpcIn);

			// reserva memoria y desempaqueta la matriz
			int size = m->rows * m->cols;
			m->data = new int[size];
			unpackv(rpcIn, m->data, size);

			// desempaqueta la dimension del string
			int s = unpack<int>(rpcIn);
			// reserva memoria y desempaqeta el string del nombre del fichero
			std::string fileName;
			fileName.resize(s);
			unpackv(rpcIn, (char *)fileName.data(), s);

			// llama a writeMatrix()
			p->writeMatrix(m, (char *)fileName.data());
			
			// emapqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case createIdentityOp:
		{
			// desempaqueta las filas y columnas
			int rows = unpack<int>(rpcIn);
			int cols = unpack<int>(rpcIn);

			// declaara una nueva matriz y la iguala al resultado de createIdentity()
			matrix_t *mres = p->createIdentity(rows, cols);

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);
			
			// empaqueta filas y columnas
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);

			// empaqueta la matriz
			int size = mres->rows * mres->cols;
			packv(rpcOut, mres->data, size);
		}
		break;

		case createRandMatrixOp:
		{
			// desempaqueta las filas y columnas
			int rows = unpack<int>(rpcIn);
			int cols = unpack<int>(rpcIn);

			// declaara una nueva matriz y la iguala al resultado de createRandMatrix()
			matrix_t *mres = p->createRandMatrix(rows, cols);

			// empaqueta el ok
			pack(rpcOut, (unsigned char)MSG_OK);
			
			// empaqueta filas y columnas
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);

			// empaqueta la matriz
			int size = mres->rows * mres->cols;
			packv(rpcOut, mres->data, size);
		}
		break;

		default:
		{
			std::cout << "Error: función no definida\n";
			pack(rpcOut, (unsigned char)MSG_NOK);
		}
		break;
		};
		// devuelve resultados
		sendMSG(clientId, rpcOut);
	};
};
