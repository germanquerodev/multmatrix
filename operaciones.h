#pragma once
#include <vector>
#include <cstring>
#include <iostream>

#define MSG_OK 1
#define MSG_NOK 0



typedef enum matrixOp
{
	constructorOp = 1,
	destructorOp = 2,
	readMatrixOp = 3,
	multMatricesOp = 4,
	writeMatrixOp = 5,
	createIdentityOp = 6,
	createRandMatrixOp = 7

} matrixOp;

template <typename T>
inline void pack(std::vector<unsigned char> &packet, T data)
{

	int size = packet.size();
	unsigned char *ptr = (unsigned char *)&data;
	packet.resize(size + sizeof(T));
	std::copy(ptr, ptr + sizeof(T), packet.begin() + size);
}

template <typename T>
inline void packv(std::vector<unsigned char> &packet, T *data, int dataSize)
{
	for (int i = 0; i < dataSize; i++)
		pack(packet, data[i]);
}

template <typename T>
inline T unpack(std::vector<unsigned char> &packet)
{
	T data;
	int dataSize = sizeof(T);
	int packetSize = packet.size();
	T *ptr = (T *)packet.data();
	data = ptr[0];

	for (int i = dataSize; i < packetSize; i++)
	{
		packet[i - dataSize] = packet[i];
	}

	packet.resize(packetSize - dataSize);
	return data;
}

template <typename T>
inline void unpackv(std::vector<unsigned char> &packet, T *data, int dataSize)
{
	for (int i = 0; i < dataSize; i++)
		data[i] = unpack<T>(packet);
}
