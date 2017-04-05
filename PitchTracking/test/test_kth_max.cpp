//
//  test_kth_order_statistic.cpp
//  multipath_test
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//

#include <stdio.h>
#include "gtest/gtest.h"
#include <random>
#include <vector>
#include <algorithm>
#include "kth_order_statistic.h"

TEST(kth_order_statistic, findNthMax) {
    
    int N = 2048;
    std::vector<double> x (N, 0.f);
    std::vector<double> aux (N, 0.f);
    std::vector<double> x_sorted (N, 0.f);
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.f, 10.f);
    
    // initialize with uniform distribution [0, 10]
    for (int i = 0; i < N; i++) {
        x[i] = distribution(generator);
    }
    
    std::copy(x.begin(), x.end(), x_sorted.begin());
    std::sort(x_sorted.begin(), x_sorted.end());
    std::reverse(x_sorted.begin(), x_sorted.end());
    
    for (int i = 0; i < N; i++) {
        int idx = findKthMax(x, aux, i);
        ASSERT_EQ(x[idx], x_sorted[i]);
    }
}