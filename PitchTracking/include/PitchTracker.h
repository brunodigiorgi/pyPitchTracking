//
//  PitchTracker.h
//  PitchTracking
//
//  Created by Bruno Di Giorgi on 19/01/16.
//  Copyright © 2016 Bruno Di Giorgi. All rights reserved.
//
//  Inspired by https://code.soundsoftware.ac.uk/projects/pyin
//  see “pYIN: A Fundamental Frequency Estimator
//  Using Probabilistic Threshold Distributions”
//  M. Mauch and S. Dixon

#ifndef PitchTracker_h
#define PitchTracker_h

#include <stdio.h>
#include <vector>

class PitchTrackerYin {
public:
    PitchTrackerYin(float sample_rate, float max_freq, float thresh, float min_weight);
    void setSampleRate(float sample_rate);
    void setMaxFreq(float max_freq);
    
    void process(float*x, int n, float *p, float *a);
    void process_prob(float*x, int n, float **out_p, float **out_prob, int *out_n);
    void reset();

private:
    void updateMinPer();
    
    float sample_rate {0.f};
    float max_freq {0.f};
    int min_per {0};
    float thresh {0.01};
    float min_weight {0.01};
    int prev_per {-1};
    std::vector<float> d;
    std::vector<int> cand_per_int;
    std::vector<float> cand_per;
    std::vector<float> cand_prob;
};

#endif /* PitchTracker_h */
