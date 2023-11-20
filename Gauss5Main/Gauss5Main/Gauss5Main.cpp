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
extern "C" int gaussTrfAsm(uint8_t * beginPtr, uint8_t * endPtr, int width);
 
int main()
{
    int threads = 4;


    BMP test("t2_24.bmp");
    BMP testC("t2_24.bmp");
    //while (test.data.size() % threads != 0) threads--; //Pętla obcinająca ilość wątków do takiej, której uda się równo obsłużyć
    int overflow = test.data_size % threads;
    int usedOverflow = 0;
    std::vector<std::thread> handler;
    std::vector<std::thread> handlerC;
    //if (overflow != 0) std::cout << "Nierowne thready\n";
    Timer::start();
    for (int i = 0; i < threads; i++) {
        if (overflow != 0) {
            handler.push_back(std::thread(gaussTrfAsm, test.beginData + i * test.data_size / threads + usedOverflow, (test.beginData + (i + 1) * test.data_size / threads) + usedOverflow, test.bmp_info_header.width));
            overflow--;
            usedOverflow++;
        } 
        else {
            handler.push_back(std::thread(gaussTrfAsm, test.beginData + i * test.data_size / threads + usedOverflow, (test.beginData + (i + 1) * test.data_size / threads) - 1 + usedOverflow, test.bmp_info_header.width));
        }
    }
    for (int i = 0; i < threads; i++) {
        handler[i].join();
    }
    //gaussTrfAsm(test.beginData, test.beginData + test.data_size, test.bmp_info_header.width);
    Timer::stop();
    int overflowC = testC.data_size % threads;
    int usedOverflowC = 0;
    //if (overflowC != 0) std::cout << "Nierowne thready\n";
    Timer::start();
    for (int i = 0; i < threads; i++) {
        if (overflowC != 0) {
            handlerC.push_back(std::thread(gaussTrf, testC.beginData + i * testC.data_size / threads + usedOverflowC, (testC.beginData + (i + 1) * testC.data_size / threads) + usedOverflowC, testC.bmp_info_header.width));
            
            overflowC--;
            usedOverflowC++;
        }
        else {
            handlerC.push_back(std::thread(gaussTrf, testC.beginData + i * testC.data_size / threads + usedOverflowC, (testC.beginData + (i + 1) * testC.data_size / threads) - 1 + usedOverflowC, testC.bmp_info_header.width));
        }
    }
    for (int i = 0; i < threads; i++) {
        handlerC[i].join();
    }
    //basicTrf(testC.data.data(), testC.data.data() + testC.data.size() - 1);
    Timer::stop();

    test.write("kopia.bmp");
    testC.write("kopiaC.bmp");
}
