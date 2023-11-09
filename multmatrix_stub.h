#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "operaciones.h"

typedef struct matrix_t
{
	int rows;
	int cols;
	int *data;
} matrix_t;

void recvMatrix(int serverId, matrix_t &m, const char *fileName, matrixOp op)
{
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	pack(rpcOut, op);

	int tam = strlen(fileName);
	pack(rpcOut, tam);
	packv(rpcOut, fileName, strlen(fileName));
	

	sendMSG(serverId, rpcOut);

	recvMSG(serverId, rpcIn);

	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK)
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else
	{
		m.rows = unpack<int>(rpcIn);
		m.cols = unpack<int>(rpcIn);

		int size = m.rows * m.cols;
		m.data = new int[size];
		unpackv(rpcIn, m.data, size);
	}
}

void sendWriteMatrix(int serverId, matrix_t &m, const char *fileName, matrixOp op)
{
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	pack(rpcOut, op);

	pack(rpcOut, m.rows);
	pack(rpcOut, m.cols);

	int size = m.rows * m.cols;
	packv(rpcOut, m.data, size);

	int tam = strlen(fileName);
	pack(rpcOut, tam);
	packv(rpcOut, fileName, strlen(fileName));

	sendMSG(serverId, rpcOut);

	recvMSG(serverId, rpcIn);

	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK)
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
}

void recvMultipliedMatrix(int serverId, matrix_t &m1, matrix_t &m2, matrix_t &mres, matrixOp op)
{
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	pack(rpcOut, op);

	pack(rpcOut, m1.rows);
	pack(rpcOut, m1.cols);

	int size = m1.rows * m1.cols;
	packv(rpcOut, m1.data, size);

	pack(rpcOut, m2.rows);
	pack(rpcOut, m2.cols);

	size = m2.rows * m2.cols;
	packv(rpcOut, m2.data, size);

	sendMSG(serverId, rpcOut);

	recvMSG(serverId, rpcIn);

	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK)
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else
	{
		mres.rows = unpack<int>(rpcIn);
		mres.cols = unpack<int>(rpcIn);

		int size = mres.rows * mres.cols;
		mres.data = new int[size];
		unpackv(rpcIn, mres.data, size);
	}
}

void recvGeneratedMatrix(int serverId, int rows, int cols, matrix_t &mres, matrixOp op)
{
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	pack(rpcOut, op);

	pack(rpcOut, rows);
	pack(rpcOut, cols);

	sendMSG(serverId, rpcOut);

	recvMSG(serverId, rpcIn);

	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK)
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else
	{
		mres.rows = unpack<int>(rpcIn);
		mres.cols = unpack<int>(rpcIn);

		int size = mres.rows * mres.cols;
		mres.data = new int[size];
		unpackv(rpcIn, mres.data, size);
	}
}

class multMatrix
{
private:
	std::string ip = "127.0.0.1";
	int port = 60000;
	connection_t serverConnection;

public:
	multMatrix()
	{
		// conectar con servidor
		serverConnection = initClient(ip, port);
		matrixOp op = constructorOp;
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		pack(rpcOut, op);

		sendMSG(serverConnection.serverId, rpcOut);
		recvMSG(serverConnection.serverId, rpcIn);
		if (rpcIn[0] != MSG_OK)
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
	}

	~multMatrix()
	{
		// cerrar conexión
		matrixOp op = destructorOp;
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		pack(rpcOut, op);

		sendMSG(serverConnection.serverId, rpcOut);
		recvMSG(serverConnection.serverId, rpcIn);
		if (rpcIn[0] != MSG_OK)
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
		closeConnection(serverConnection.serverId);
	}

	matrix_t *createIdentity(int rows, int cols)
	{
		matrix_t *mres = new matrix_t();
		recvGeneratedMatrix(serverConnection.serverId, rows, cols, *mres, createIdentityOp);
		return mres;
	}

	matrix_t *createRandMatrix(int rows, int cols)
	{
		matrix_t *mres = new matrix_t();
		recvGeneratedMatrix(serverConnection.serverId, rows, cols, *mres, createRandMatrixOp);
		return mres;
	}

	matrix_t *multMatrices(matrix_t *m1, matrix_t *m2)
	{
		matrix_t *mres = new matrix_t();
		recvMultipliedMatrix(serverConnection.serverId, *m1, *m2, *mres, multMatricesOp);
		return mres;
	}

	void writeMatrix(matrix_t *m, const char *fileName)
	{
		sendWriteMatrix(serverConnection.serverId, *m, fileName, writeMatrixOp);
	}

	matrix_t *readMatrix(const char *fileName)
	{
		matrix_t *mres = new matrix_t();
		recvMatrix(serverConnection.serverId, *mres, fileName, readMatrixOp);
		return mres;
	}
};
