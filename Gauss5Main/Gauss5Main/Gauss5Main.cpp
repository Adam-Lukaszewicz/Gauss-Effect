#include <iostream>
#include "transformationCPP.h"
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <random>
#include "Bitmap.h"
#include "timer.h"

extern "C" int basicTrfAsm(uint8_t* beginPtr, uint8_t * endPtr);

int main()
{
    int threads = 2;


    BMP test("t2_24.bmp");
    BMP testC("sample4.bmp");
    //while (test.data.size() % threads != 0) threads--; //Pętla obcinająca ilość wątków do takiej, której uda się równo obsłużyć
    int overflow = test.data.size() % threads;
    int usedOverflow = 0;
    std::vector<std::jthread> handler;
    //if (overflow != 0) std::cout << "Nierowne thready\n";
    //Timer::start();
    //for (int i = 0; i < threads; i++) {
    //    if (overflow != 0) {
    //        handler.push_back(std::jthread(basicTrfAsm, test.data.data() + i * test.data.size() / threads + usedOverflow, (test.data.data() + (i + 1) * test.data.size() / threads) + usedOverflow));
    //        overflow--;
    //        usedOverflow++;
    //    } 
    //    else {
    //        handler.push_back(std::jthread(basicTrfAsm, test.data.data() + i * test.data.size() / threads + usedOverflow, (test.data.data() + (i + 1) * test.data.size() / threads) - 1 + usedOverflow));
    //    }
    //}
    //Timer::stop();
    int overflowC = testC.data.size() % threads;
    int usedOverflowC = 0;
    //if (overflowC != 0) std::cout << "Nierowne thready\n";
    Timer::start();
    for (int i = 0; i < threads; i++) {
        if (overflowC != 0) {
            handler.push_back(std::jthread(gaussTrf, testC.beginData + i * testC.data_size / threads + usedOverflowC, (testC.beginData + (i + 1) * testC.data_size / threads) + usedOverflowC, testC.bmp_info_header.width));
            
            overflowC--;
            usedOverflowC++;
        }
        else {
            handler.push_back(std::jthread(gaussTrf, testC.beginData + i * testC.data_size / threads + usedOverflowC, (testC.beginData + (i + 1) * testC.data_size / threads) - 1 + usedOverflowC, testC.bmp_info_header.width));
        }
    }
    //basicTrf(testC.data.data(), testC.data.data() + testC.data.size() - 1);
    Timer::stop();

    test.write("kopia.bmp");
    testC.write("kopiaC.bmp");
}
