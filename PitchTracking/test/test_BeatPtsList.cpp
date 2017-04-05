//
//  test_BeatPtsList.cpp
//  multipath_test
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"
#include "BeatPtsList.h"

#include <random>
#include <vector>
#include <algorithm>

TEST(BeatPtsList, obsScore) {
    
    int N = 12;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    
    BeatPtsList bpl(array, m, n, true);
    
    for (int i = 0; i < n; i++)
        ASSERT_EQ(bpl.p_time[i], array[0 * n + i]);
    
    for (int i = 0; i < n; i++)
        ASSERT_EQ(bpl.p_obs[i], array[1 * n + i]);
    
    for (int i = 0; i < n; i++)
        ASSERT_EQ(bpl.p_period[i], array[2 * n + i]);
    
    delete[] array;
}

TEST(BeatPtsList, transScore) {
    
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    float period = 10.5f;
    int ind = 4;
    
    BeatPtsList bpl(array, m, n, true);
    
    for (int i = 0; i < n; i++) {
        bpl.p_time[i] = i;
        bpl.p_period[i] = period;
    }
    
    std::vector<int> cand_ind (N, 0);
    std::vector<double> cand_ts (N, 0);
    cand_ind.resize(0);
    cand_ts.resize(0);
    bpl.transition_score(ind, &cand_ind, &cand_ts);
    
    int ncand = static_cast<int>(cand_ind.size());
    for (int i = 0; i < ncand; i++) {
        int i_cand = cand_ind[i];
        double mean_p = (bpl.p_period[i_cand] + bpl.p_period[ind] ) * .5;
        double delta = fabs(bpl.p_time[i_cand] - bpl.p_time[ind]);
        double ts = -pow(log2(delta/mean_p), 2);
        ASSERT_EQ(ts, cand_ts[i]);
    }
    
    delete[] array;
    
}

TEST(BeatPtsList, limitCase1) {
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    float period = 10.5f;
    int ind = n-1;
    
    BeatPtsList bpl(array, m, n, true);
    
    for (int i = 0; i < n; i++) {
        bpl.p_time[i] = i;
        bpl.p_period[i] = period;
    }
    
    std::vector<int> cand_ind (N, 0);
    std::vector<double> cand_ts (N, 0);
    cand_ind.resize(0);
    cand_ts.resize(0);
    bpl.transition_score(ind, &cand_ind, &cand_ts);
    
    ASSERT_EQ(1, cand_ts.size());
    ASSERT_EQ(-1, cand_ind[0]);
    ASSERT_EQ(0, cand_ts[0]);
    
    delete[] array;
    
}

TEST(BeatPtsList, limitCase2) {
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    float period = 10.5f;
    int ind = n-period+1;
    
    BeatPtsList bpl(array, m, n, true);
    
    for (int i = 0; i < n; i++) {
        bpl.p_time[i] = i;
        bpl.p_period[i] = period;
    }
    
    std::vector<int> cand_ind (N, 0);
    std::vector<double> cand_ts (N, 0);
    cand_ind.resize(0);
    cand_ts.resize(0);
    bpl.transition_score(ind, &cand_ind, &cand_ts);
    
    int ncand = static_cast<int>(cand_ts.size());
    ASSERT_GE(ncand, 1);
    ASSERT_EQ(-1, cand_ind[ncand-1]);
    ASSERT_EQ(0, cand_ts[ncand-1]);
    
    delete[] array;
}

