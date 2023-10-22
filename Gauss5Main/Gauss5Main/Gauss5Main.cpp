// Gauss5Main.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "transformationCPP.h"

extern "C" int MyProc(int a, int b);

int main()
{
    std::cout << test(3) << std::endl;
    std::cout << MyProc(1, 2) << std::endl;
}
