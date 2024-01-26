#include "pch.h"
#include "transformationCPP.h"
#include <stdexcept>

int basicTrf(uint8_t* beginPtr, uint8_t* endPtr) {
	while (beginPtr != endPtr) {
		*(beginPtr) += 100;
		beginPtr++;
	}
	return 1;
}

int gaussTrf(uint8_t* beginPtr, uint8_t* endPtr, uint8_t* copyBegin, int width) {
	//beginPtr += 6 * width + 6;
	//endPtr -= 6 * width - 6;
	uint8_t* iteratorPtr = beginPtr;
	uint8_t* copyPtr = copyBegin;
	
	while (iteratorPtr != endPtr)
	{

		*(copyPtr) = (*(iteratorPtr - 6) + *(iteratorPtr - 3) * 4 + *(iteratorPtr) * 6 + *(iteratorPtr + 3) * 4 + *(iteratorPtr + 6))>>4;
		iteratorPtr++;
		copyPtr++;
	}
	iteratorPtr = beginPtr;
	copyPtr = copyBegin;
	while (iteratorPtr != endPtr)
	{
		*(iteratorPtr) = (*(copyPtr - width * 6) + *(copyPtr - width * 3) * 4 + *(copyPtr) * 6 + *(copyPtr + width * 3) * 4 + *(copyPtr + width * 6))>>4;
		iteratorPtr++;
		copyPtr++;
	}
	
	/*
	int matrix[5][5] = { {1, 4, 6, 4, 1 }, {4, 16, 24, 16, 4}, {6, 24, 36, 24, 6}, {4, 16, 24, 16, 4}, {1, 4, 6, 4, 1} };
	while (iteratorPtr != endPtr) {
		int result = 0;
		for (int i = -6; i < 7; i+=3) {
			for (int j = -6; j < 7; j += 3) {
				result += *(iteratorPtr + (width * i) + j) * matrix[(i / 3) + 2][(j / 3) + 2];
			}
		}
		result /= 256;
		*(iteratorPtr) = result;
		iteratorPtr++;
	}
	*/
	return 1;
}