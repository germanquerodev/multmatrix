#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

void printRpc(const std::vector<unsigned char> &rpc)
{
	std::cout << "Contenido de rpc:\n";
	for (const auto &byte : rpc)
	{
		std::cout << static_cast<int>(byte) << " "; // Imprime el valor numérico de cada byte
	}
	std::cout << std::endl;
}

void printMatrix(const matrix_t &matrix)
{
	std::cout << "rows: " << matrix.rows << "\n";
	std::cout << "cols: " << matrix.cols << "\n";
	std::cout << "matrix:\n";
	for (int i = 0; i < matrix.rows; ++i)
	{
		for (int j = 0; j < matrix.cols; ++j)
		{
			int index = i * matrix.cols + j;
			std::cout << matrix.data[index] << " ";
		}
		std::cout << "\n";
	}
}

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
		std::vector<unsigned char> rpcIn;
		std::vector<unsigned char> rpcOut;
		// recibe operación
		recvMSG(clientId, rpcIn);

		matrixOp operacion = unpack<matrixOp>(rpcIn);

		// ejecuta
		switch (operacion)
		{
		case constructorOp:
		{

			p = new multMatrix();
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case destructorOp:
		{
			delete p;
			p = nullptr;
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case readMatrixOp:
		{
			int s = unpack<int>(rpcIn);
			std::string fileName;
			fileName.resize(s);
			unpackv(rpcIn, (char *)fileName.data(), s);
			matrix_t *m = p->readMatrix((char *)fileName.data());
			pack(rpcOut, (unsigned char)MSG_OK);
			pack(rpcOut, m->rows);
			pack(rpcOut, m->cols);
			s = m->rows * m->cols;
			packv(rpcOut, m->data, s);
		}
		break;
		case multMatricesOp:
		{
			matrix_t *m1 = new matrix_t[1];
			m1->rows = unpack<int>(rpcIn);
			m1->cols = unpack<int>(rpcIn);
			int size = m1->rows * m1->cols;
			m1->data = new int[size];
			unpackv(rpcIn, m1->data, size);
			matrix_t *m2 = new matrix_t[1];
			m2->rows = unpack<int>(rpcIn);
			m2->cols = unpack<int>(rpcIn);
			size = m2->rows * m2->cols;
			m2->data = new int[size];
			unpackv(rpcIn, m2->data, size);

			matrix_t *mres = p->multMatrices(m1, m2);
			pack(rpcOut, (unsigned char)MSG_OK);
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);
			size = mres->rows * mres->cols;
			packv(rpcOut, mres->data, size);
		}
		break;

		case writeMatrixOp:
		{
			matrix_t *m = new matrix_t[1];
			m->rows = unpack<int>(rpcIn);
			m->cols = unpack<int>(rpcIn);
			int size = m->rows * m->cols;
			m->data = new int[size];
			unpackv(rpcIn, m->data, size);
			int tam = unpack<int>(rpcIn);
			std::string fileName;
			fileName.resize(tam);
			unpackv(rpcIn, (char *)fileName.data(), tam);
			p->writeMatrix(m, (char *)fileName.data());
			pack(rpcOut, (unsigned char)MSG_OK);
		}
		break;
		case createIdentityOp:
		{
			int rows = unpack<int>(rpcIn);
			int cols = unpack<int>(rpcIn);
			matrix_t *mres = p->createIdentity(rows, cols);
			pack(rpcOut, (unsigned char)MSG_OK);
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);
			int size = mres->rows * mres->cols;
			packv(rpcOut, mres->data, size);
		}
		break;

		case createRandMatrixOp:
		{
			int rows = unpack<int>(rpcIn);
			int cols = unpack<int>(rpcIn);
			matrix_t *mres = p->createRandMatrix(rows, cols);
			pack(rpcOut, (unsigned char)MSG_OK);
			pack(rpcOut, mres->rows);
			pack(rpcOut, mres->cols);
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
