#pragma once

#ifdef TRANSFORMATIONCPP_EXPORTS
#define TRANSFORMATIONCPP_API __declspec(dllexport)
#else
#define TRANSFORMATIONCPP_API __declspec(dllimport)
#endif

extern "C" TRANSFORMATIONCPP_API int test(int a);
