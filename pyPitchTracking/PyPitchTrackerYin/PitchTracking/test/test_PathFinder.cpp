//
//  test_PathFinder.cpp
//  multipath_test
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"
#include "PathFinderMP.h"
#include "PathFinderDP.h"

TEST(PathFinderMP, trivial_path){
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    int period_int = 10;
    float period = period_int;
    
    for (int i = 0; i < n; i++) {
        array[n+i] = 0.f;
        if (i%period_int == 0) {
            array[n+i] = 0.1;
        }
        array[i] = i;
        array[2*n+i] = period;
    }
    
    PathFinderMP pf(4, 1);
    pf.setPtsList(array, m, n);
    
    int * path;
    int path_len;
    pf.findPath(&path, &path_len);
    
    for (int i = 0; i < path_len; i++) {
        ASSERT_EQ(period_int * i, path[i]);
    }
    
    delete[] array;
}

TEST(PathFinderDP, trivial_path){
    int N = 128;
    float * array = new float[N];
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }
    
    int m = 3;
    int n = N/m;
    int period_int = 10;
    float period = period_int;
    
    for (int i = 0; i < n; i++) {
        array[n+i] = 0.f;
        if (i%period_int == 0) {
            array[n+i] = 0.1;
        }
        array[i] = i;
        array[2*n+i] = period;
    }
    
    PathFinderDP pf(1);
    pf.setPtsList(array, m, n);
    
    int * path;
    int path_len;
    pf.findPath(&path, &path_len);
    
    for (int i = 0; i < path_len; i++) {
        ASSERT_EQ(period_int * i, path[i]);
    }
    
    delete[] array;
}