#pragma once
#include <stdexcept>
#ifdef TRANSFORMATIONCPP_EXPORTS
#define TRANSFORMATIONCPP_API __declspec(dllexport)
#else
#define TRANSFORMATIONCPP_API __declspec(dllimport)
#endif

extern "C" TRANSFORMATIONCPP_API int basicTrf(uint8_t* beginPtr, uint8_t* endPtr);

extern "C" TRANSFORMATIONCPP_API int gaussTrf(uint8_t * beginPtr, uint8_t * endPtr, uint8_t * copyBegin, int width);
