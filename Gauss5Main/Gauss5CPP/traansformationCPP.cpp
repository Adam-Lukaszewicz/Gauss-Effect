#include "pch.h"
#include "transformationCPP.h"
#include <stdexcept>

int basicTrf(uint8_t* beginPtr, uint8_t* endPtr) {
	while (beginPtr != endPtr) {
		*(beginPtr) += 100;
		if (*(beginPtr) > 255) *(beginPtr) -= 255;
		beginPtr++;
	}
	return 1;
}