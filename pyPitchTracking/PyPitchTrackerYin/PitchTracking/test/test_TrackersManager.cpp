//
//  test_TrackersManager.cpp
//  multipath_test
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include "TrackersManager.h"
#include "BeatPtsList.h"

TEST(TrackersManager, trivial_path){
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    int period_int = 10;
    float period = period_int;
    
    BeatPtsList bpl(array, m, n, true);
    
    for (int i = 0; i < n; i++) {
        bpl.p_obs[i] = 0.f;
        if (i%period_int == 0) {
            bpl.p_obs[i] = 0.1;
        }
        bpl.p_time[i] = i;
        bpl.p_period[i] = period;
    }
    
    TrackersManager tm(4, 1);
    tm.setPtsList(bpl);
    
    std::vector<int> * path;
    tm.find_path(&path);
    
    int path_len = static_cast<int>(path->size());
    for (int i = 0; i < path_len; i++) {
        ASSERT_EQ(period_int * i, (*path)[i]);
    }
    
    delete[] array;
}