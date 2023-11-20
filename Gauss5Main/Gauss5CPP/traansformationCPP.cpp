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

int gaussTrf(uint8_t* beginPtr, uint8_t* endPtr, int width) {
	//beginPtr += 6 * width + 6;
	//endPtr -= 6 * width - 6;
	width += 4;
	uint8_t* iteratorPtr = beginPtr;
	while (iteratorPtr != endPtr)
	{

		*(iteratorPtr) = (*(iteratorPtr - 6) + *(iteratorPtr - 3) * 4 + *(iteratorPtr) * 6 + *(iteratorPtr + 3) * 4 + *(iteratorPtr + 6))>>4;
		iteratorPtr++;
	}
	iteratorPtr = beginPtr;
	while (iteratorPtr != endPtr)
	{
		*(iteratorPtr) = (*(iteratorPtr - width * 6) + *(iteratorPtr - width * 3) * 4 + *(iteratorPtr) * 6 + *(iteratorPtr + width * 3) * 4 + *(iteratorPtr + width * 6))>>4;
		iteratorPtr++;
	}
	return 1;
}