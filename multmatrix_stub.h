#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "operaciones.h"

// estructura de matrix
typedef struct matrix_t
{
	int rows;
	int cols;
	int *data;
} matrix_t;

// recibe el id del servidor, un objeto matriz, un nombre de archivo y una operacion
// empaqueta el nombre de archivo y la operacion para hacer una llamada al servidor
// el servidor devuelve un ok y una matriz empaquetada
// lo desempaqueta y lo guarda dentro del objeto matriz dado
void recvMatrix(int serverId, matrix_t &m, const char *fileName, matrixOp op)
{
	// inicializa rpcs
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	// empaqueta operacion
	pack(rpcOut, op);

	// empaqueta dimension del string del nombre de archivo
	int tam = strlen(fileName);
	pack(rpcOut, tam);
	// empaqueta el string del nombre de archivo
	packv(rpcOut, fileName, tam);

	// envia el paquete al servidor
	sendMSG(serverId, rpcOut);
	// recibe el paquete del servidor
	recvMSG(serverId, rpcIn);

	// comprueba el ok
	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK) // si no es ok salta el error
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else // si es ok:
	{
		// desempaqueta las filas y columnas
		m.rows = unpack<int>(rpcIn);
		m.cols = unpack<int>(rpcIn);

		// reserva memoria y desempaqueta la matriz
		int size = m.rows * m.cols;
		m.data = new int[size];
		unpackv(rpcIn, m.data, size);
	}
}

// recibe el id del servidor, un objeto matriz, un nombre de archivo y una operacion
// empaqueta el nombre de archivo y la operacion para hacer una llamada al servidor
// el servidor devuelve un ok
void sendMatrix(int serverId, matrix_t &m, const char *fileName, matrixOp op)
{
	// inicializa rpcs
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	// empaqueta operacion
	pack(rpcOut, op);

	// empaqueta filas y columnas
	pack(rpcOut, m.rows);
	pack(rpcOut, m.cols);

	// empaqueta la matriz
	int size = m.rows * m.cols;
	packv(rpcOut, m.data, size);

	// empaqueta dimension del string del nombre de archivo
	int tam = strlen(fileName);
	pack(rpcOut, tam);
	// empaqueta el string del nombre de archivo
	packv(rpcOut, fileName, tam);

	// envia el paquete al servidor
	sendMSG(serverId, rpcOut);
	// recibe el paquete del servidor
	recvMSG(serverId, rpcIn);

	// comprueba el ok
	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK) // si no es ok salta error
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
}

// recibe el id del servidor, tres objetos matriz y una operacion
// empaqueta las dos primeras matrices y la operacion para hacer una llamada al servidor
// el servidor devuelve un ok y una matriz empaquetada
// lo desempaqueta y lo guarda dentro del tercer objeto matriz dado
void recvOpMatrix(int serverId, matrix_t &m1, matrix_t &m2, matrix_t &mres, matrixOp op)
{
	// inicializa rpcs
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	// empaqueta operacion
	pack(rpcOut, op);

	// M1
	// empaqueta filas y columnas
	pack(rpcOut, m1.rows);
	pack(rpcOut, m1.cols);
	// empaqueta la matriz
	int size = m1.rows * m1.cols;
	packv(rpcOut, m1.data, size);

	// M2
	// empaqueta filas y columnas
	pack(rpcOut, m2.rows);
	pack(rpcOut, m2.cols);
	// empaqueta la matriz
	size = m2.rows * m2.cols;
	packv(rpcOut, m2.data, size);

	// envia el paquete al servidor
	sendMSG(serverId, rpcOut);
	// recibe el paquete del servidor
	recvMSG(serverId, rpcIn);

	// comprueba el ok
	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK) // si no es ok salta error
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else // si es ok:
	{
		// desempaqueta las filas y columans
		mres.rows = unpack<int>(rpcIn);
		mres.cols = unpack<int>(rpcIn);

		// reserva memoria y desempaqueta la matriz
		int size = mres.rows * mres.cols;
		mres.data = new int[size];
		unpackv(rpcIn, mres.data, size);
	}
}

// recibe el id del servidor, dos ints y un objeto matrix
// empaqueta los dos ints y la operacion para hacer una llamada al servidor
// el servidor devuelve un ok y una matriz empaquetada
// lo desempaqueta y lo guarda dentro del objeto matriz dado
void recvGenMatrix(int serverId, int rows, int cols, matrix_t &mres, matrixOp op)
{
	// inicializa rpcs
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	// empaqueta operacion
	pack(rpcOut, op);

	// empaqueta filas y columnas
	pack(rpcOut, rows);
	pack(rpcOut, cols);

	// envia el paquete al servidor
	sendMSG(serverId, rpcOut);
	// recibe el paquete del servidor
	recvMSG(serverId, rpcIn);

	// comprueba el ok
	unsigned char ok = unpack<unsigned char>(rpcIn);
	if (ok != MSG_OK) // si no es ok salta error
	{
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}
	else // si es ok:
	{
		// desempaqueta las filas y columans
		mres.rows = unpack<int>(rpcIn);
		mres.cols = unpack<int>(rpcIn);

		// reserva memoria y desempaqueta la matriz
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

		// inicializa la operacion para llamar al constructor en el servidor
		matrixOp op = constructorOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		// empaqeta la operacion
		pack(rpcOut, op);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);
		
		// comprueba el ok
		if (rpcIn[0] != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}
	}

	~multMatrix()
	{
		// inicializa la operacion para llamar al destructor en el servidor
		matrixOp op = destructorOp;

		// inicializa rpcs
		std::vector<unsigned char> rpcOut;
		std::vector<unsigned char> rpcIn;

		// empaqeta la operacion
		pack(rpcOut, op);

		// envia el parquete al servidor
		sendMSG(serverConnection.serverId, rpcOut);
		// recibe el paquete del servidor
		recvMSG(serverConnection.serverId, rpcIn);

		// comprueba el ok
		if (rpcIn[0] != MSG_OK) // si no es ok salta error
		{
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
		}

		// cierra la conexion
		closeConnection(serverConnection.serverId);
	}

	// maneja la llamada a recvGenMatrix para que funcione con la operacion createIdentityOp
	// devuleve la respuesta al cliente
	matrix_t *createIdentity(int rows, int cols)
	{
		matrix_t *mres = new matrix_t();
		recvGenMatrix(serverConnection.serverId, rows, cols, *mres, createIdentityOp);
		return mres;
	}

	// maneja la llamada a recvGenMatrix para que funcione con la operacion createRandMatrix
	// devuelve la respuesta al cliente
	matrix_t *createRandMatrix(int rows, int cols)
	{
		matrix_t *mres = new matrix_t();
		recvGenMatrix(serverConnection.serverId, rows, cols, *mres, createRandMatrixOp);
		return mres;
	}

	// maneja la llamada a recvOpMatrix para que funcione con la operacion multMatricesOp
	// devuelve la respuesta al cliente
	matrix_t *multMatrices(matrix_t *m1, matrix_t *m2)
	{
		matrix_t *mres = new matrix_t();
		recvOpMatrix(serverConnection.serverId, *m1, *m2, *mres, multMatricesOp);
		return mres;
	}

	// maneja la llamada a sendMatrix para que funcione con la operacion writeMatrixOp
	// no devuelve nada, la respuesta tan solo sirve para que salte el error en sendMatrix
	void writeMatrix(matrix_t *m, const char *fileName)
	{
		sendMatrix(serverConnection.serverId, *m, fileName, writeMatrixOp);
	}

	// maneja la llamada a recvMatrix para que funcione con la operacion readMatrixOp
	// devuelve la respuesta al cliente
	matrix_t *readMatrix(const char *fileName)
	{
		matrix_t *mres = new matrix_t();
		recvMatrix(serverConnection.serverId, *mres, fileName, readMatrixOp);
		return mres;
	}
};
