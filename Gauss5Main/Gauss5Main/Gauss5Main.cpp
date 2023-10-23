#include <iostream>
#include "transformationCPP.h"
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <random>
#include "Bitmap.h"

extern "C" int MyProc(int a, int b);

int wrapFunc(std::vector<int>& a, std::vector<int>& b, int beg, int end) {
    int max = 0;
    for (int i = beg; i < end; i++) {
        if (MyProc(a[i], b[i]) > max) max = MyProc(a[i], b[i]);
    }
    return max;
}

int main()
{
    int threads = 2;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(5, 500);
    std::uniform_int_distribution<std::mt19937::result_type> size(6, 24);

    int n = size(rng);
    std::vector<int> a, b;
    for (int i = 0; i < n; i++) {
        a.push_back(dist(rng));
        b.push_back(dist(rng));
    }

    BMP test("Shapes.bmp", threads);
    test.write("kopia.bmp");
    BMP unevenTest("t2_24.bmp", threads);
    unevenTest.write("unevenkopia.bmp");

    int actualThreads = threads;

    while (n % actualThreads != 0) {
        actualThreads--;
    }
    std::vector<std::future<int>> futures;

    for (int i = 0; i < actualThreads; i++) {
        futures.push_back(std::async(wrapFunc, std::ref(a), std::ref(b), 0 + n / actualThreads * i, n / actualThreads + n / actualThreads * i));
    }
    for (int i = 0; i < futures.size(); i++) {
        std::cout << futures[i].get() << "\n";
    }
}
