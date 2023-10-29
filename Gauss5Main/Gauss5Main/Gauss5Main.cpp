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
    int threads = 4;


    BMP test("t2_24.bmp", threads);
    BMP testC("t2_24.bmp", threads);
    while (test.data.size() % threads != 0) threads--; //Pętla obcinająca ilość wątków do takiej, której uda się równo obsłużyć
    Timer::start();
    std::vector<std::jthread> handler;
    for (int i = 0; i < threads; i++) {
        handler.push_back(std::jthread(basicTrfAsm, test.data.data() + i * test.data.size()/threads, (test.data.data() + (i+1) * test.data.size()/threads) - 1));
    }
    //basicTrfAsm(test.data.data(), test.data.data() + test.data.size() - 1);
    Timer::stop();
    Timer::start();
    for (int i = 0; i < threads; i++) {
        handler.push_back(std::jthread(basicTrf, testC.data.data() + i * testC.data.size() / threads, (testC.data.data() + (i + 1) * testC.data.size() / threads) - 1));
    }
    //basicTrf(testC.data.data(), testC.data.data() + testC.data.size() - 1);
    Timer::stop();

    test.write("kopia.bmp");
    testC.write("kopiaC.bmp");
}
