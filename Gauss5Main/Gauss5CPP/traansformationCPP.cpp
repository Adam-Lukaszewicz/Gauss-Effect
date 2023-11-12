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

int gaussTrf(uint8_t* beginPtr, uint8_t* endPtr) {
	return 1;
}