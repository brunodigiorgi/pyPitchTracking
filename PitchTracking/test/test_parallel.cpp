//
//  test_openmp.cpp
//  multipath_test
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"
#include <cmath>
#include <thread>
#include <vector>

void threadFunc(int N) {
    float t = 0.f;
    for (int i=0; i<N; i++) {
        t = sin(i) + cos(i) + tan(i);
    }
}

TEST(Parallel, parallel_for) {
    
    int N = 1 << 10;  // put to 24 to notice the difference
    float * array = new float[N];
    
    const unsigned int nbThreads = std::thread::hardware_concurrency();
    // std::cout << "parallel_start, " << nbThreads << " threads" << std::endl;
    
    std::vector<std::thread> threads (nbThreads);  // non running thread
    for (unsigned int idThread = 0; idThread < nbThreads; idThread++){
        threads[idThread] = std::move(std::thread(threadFunc, N));
    }
    for (auto & t : threads) t.join();
    
    // std::cout << "parallel_stop" << std::endl;
    // std::cout << "seq_start" << std::endl;
    
    for (unsigned int idThread = 0; idThread < nbThreads; idThread++){
        threadFunc(N);
    }
    
    // std::cout << "seq_stop" << std::endl;
    
    
    delete[] array;
    
}