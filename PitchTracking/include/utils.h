//
//  utils.h
//  PitchTracking
//
//  Created by Bruno Di Giorgi on 27/01/16.
//
//  Inspired by https://code.soundsoftware.ac.uk/projects/pyin
//  see “pYIN: A Fundamental Frequency Estimator
//  Using Probabilistic Threshold Distributions”
//  M. Mauch and S. Dixon

#ifndef utils_h
#define utils_h

#include <vector>

// d is assumed to be n/2 long
void cmn_df(float* x, int n, std::vector<float>& d);

int period_yin(std::vector<float>& d, float thresh, int min_per);

void period_yin_prob(std::vector<float>& d, int min_per, float minWeight,
                     std::vector<int>& out_per, std::vector<float>& out_prob);

float parab_interp(float x, float y1, float y2, float y3);

float aperiodicity(float* x, int n, float prd);

int search_local_minima_near(std::vector<float>& d, int ind);

#endif /* utils_h */
