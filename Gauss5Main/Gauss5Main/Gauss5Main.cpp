// Gauss5Main.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
//#include <Windows.h>
#include "transformationCPP.h"

//typedef int(__stdcall* ASMFunc)(int, int);

int main()
{
    std::cout << test(5);
    //HINSTANCE hGetProcIDDLL = LoadLibrary(L"C:\\Users\\jakub\OneDrive\\Pulpit\\PPK-lokal\\JA\\Proj\\Gauss5Main\\x64\\Release\\Gauss5ASM.dll");

    //if (!hGetProcIDDLL) {
    //    std::cout << "could not load the dynamic library" << std::endl;
    //    std::cout << GetLastError();
    //    return EXIT_FAILURE;
    //}


    //// resolve function address here
    //ASMFunc MyProc1 = (ASMFunc)GetProcAddress(hGetProcIDDLL, "MyProc1");
    //if (!MyProc1) {
    //    std::cout << "could not locate the function" << std::endl;
    //    return EXIT_FAILURE;
    //}

    //std::cout << "funci() returned " << MyProc1(2, 3) << std::endl;

    //return EXIT_SUCCESS;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
