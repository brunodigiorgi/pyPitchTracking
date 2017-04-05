//
//  PitchTracker.cpp
//  PitchTracking
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//
//  Inspired by https://code.soundsoftware.ac.uk/projects/pyin
//  see “pYIN: A Fundamental Frequency Estimator
//  Using Probabilistic Threshold Distributions”
//  M. Mauch and S. Dixon

#include <cassert>
#include <cmath>
#include "PitchTracker.h"
#include "utils.h"

PitchTrackerYin::PitchTrackerYin(float sample_rate_, float max_freq_, float thresh_, float min_weight_)
: sample_rate(sample_rate_), max_freq(max_freq_), thresh(thresh_), min_weight(min_weight_) {
    updateMinPer();
}

void PitchTrackerYin::setSampleRate(float sample_rate_) {
    sample_rate = sample_rate_;
    updateMinPer();
}

void PitchTrackerYin::setMaxFreq(float max_freq_) {
    max_freq = max_freq_;
    updateMinPer();
}

void PitchTrackerYin::updateMinPer() {
    min_per = floor(sample_rate / max_freq);
}

void PitchTrackerYin::process(float*x, int n, float *p, float *a) {
    cmn_df(x, n, d);
    int p_int = search_local_minima_near(d, prev_per);
    
    if(p_int == -1) {
        p_int = period_yin(d, thresh, min_per);
    }    
    
    *p = parab_interp(p_int, d[p_int-1], d[p_int], d[p_int+1]);
    *a = aperiodicity(x, n, *p);
    *p = sample_rate / *p;
}

void PitchTrackerYin::process_prob(float*x, int n, float **out_p, float **out_prob, int *out_n) {
    cmn_df(x, n, d);
    period_yin_prob(d, min_per, min_weight, cand_per_int, cand_prob);
    
    int N = cand_per_int.size();
    cand_per.resize(N);
    for (int i=0; i<N-1; i++) {
        int p_int = cand_per_int[i];
        cand_per[i] = sample_rate / parab_interp(p_int, d[p_int-1], d[p_int], d[p_int+1]);
    }
    cand_per[N-1] = cand_per_int[N-1];
    
    *out_n = N;
    *out_p = cand_per.data();
    *out_prob = cand_prob.data();
}









