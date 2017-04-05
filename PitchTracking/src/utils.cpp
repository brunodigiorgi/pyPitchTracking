//
//  utils.cpp
//  PitchTracking
//
//  Created by Bruno Di Giorgi on 27/01/16.
//
//  Inspired by https://code.soundsoftware.ac.uk/projects/pyin
//  see “pYIN: A Fundamental Frequency Estimator
//  Using Probabilistic Threshold Distributions”
//  M. Mauch and S. Dixon

#include "utils.h"
#include <cmath>

void cmn_df(float* x, int n, std::vector<float>& d) {
    float cum_sum = 0;
    int L = int(n/2);
    d.resize(L);
    
    for(int tau=0; tau<L; tau++) {
        d[tau] = 0;
        for(int i=0; i<L; i++)
            d[tau] += pow(x[i]-x[i+tau], 2);
        cum_sum += d[tau];
        
        if(cum_sum > 0)
            d[tau] = d[tau] * tau / cum_sum;
    }
    d[0] = 1;
}

int period_yin(std::vector<float>& d, float thresh, int min_per) {
    float min_val = 100000;
    int min_ind = 0;
    for(int i=min_per; i<d.size()-1; i++) {
        if (d[i] < min_val) {
            min_ind = i;
            min_val = d[i];
        }
        if(d[i] < thresh and d[i] < d[i-1] and d[i] < d[i+1]) {
            return i;
        }
    }
    return min_ind;
}

static float betaDist10[100] = {0.028911,0.048656,0.061306,0.068539,0.071703,0.071877,0.069915,0.066489,0.062117,0.057199,0.052034,0.046844,0.041786,0.036971,0.032470,0.028323,0.024549,0.021153,0.018124,0.015446,0.013096,0.011048,0.009275,0.007750,0.006445,0.005336,0.004397,0.003606,0.002945,0.002394,0.001937,0.001560,0.001250,0.000998,0.000792,0.000626,0.000492,0.000385,0.000300,0.000232,0.000179,0.000137,0.000104,0.000079,0.000060,0.000045,0.000033,0.000024,0.000018,0.000013,0.000009,0.000007,0.000005,0.000003,0.000002,0.000002,0.000001,0.000001,0.000001,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000};

void period_yin_prob(std::vector<float>& d, int min_per, float minWeight,
                     std::vector<int>& out_per, std::vector<float>& out_prob) {
    float min_val = 100000;
    int t_size = 100;
    
    int min_ind = 0;
    float sumProb = 0;
    
    int t_i = t_size - 1;
    out_per.resize(0);
    out_prob.resize(0);
    int i = min_per;
    while(i < d.size() - 1) {
        if (d[i] < 1.f && d[i+1] < d[i]) {
            
            while (i < d.size() - 1 and d[i+1]<d[i]) {
                i++;
            }
            
            if (d[i] < min_val) {
                min_ind = out_per.size();
                min_val = d[i];
            }
            
            // t_i = t_size - 1;  // reset t_i
            if(0.01 + 0.01f * t_i > d[i] && t_i > -1) {
                out_per.push_back(i);
                float p_prob = 0;
                while (0.01 + 0.01f * t_i > d[i] && t_i > -1) {
                    p_prob += betaDist10[t_i];
                    t_i--;
                }
                out_prob.push_back(p_prob);
                sumProb += p_prob;
            }
            
            i++;
        } else {
            i++;
        }
    }
    
    // update out_prob[min_ind] and add nonPeakProb
    double nonPeakProb = 1 - sumProb;
    if (min_ind > 0) {
        float delta = nonPeakProb * minWeight;
        out_prob[min_ind] += delta;
        nonPeakProb -= delta;
    }
    
    out_per.push_back(-1);
    out_prob.push_back(nonPeakProb);
}


// search local minima in d around ind
int search_local_minima_near(std::vector<float>& d, int ind) {
    int ret = -1;
    for (int i=ind-1; i<=ind+1; i++) {
        if(i > 0 and i < d.size()-1 &&
           d[i] < d[i-1] && d[i] < d[i+1])
            ret = i;
    }
    return ret;
}

float parab_interp(float x, float y1, float y2, float y3) {
    if ((y3 == y2) && (y2 == y1))
        return x;
    else
        return x + (y3-y1) / (2. * (2. * y2-y3-y1));
}

float aperiodicity(float* x, int n, float prd) {
    int prd_int = int(floor(prd));
    float prd_frac = prd - prd_int;
    
    float sum_pwr = 0;
    float sum_res = 0;
    for (int i=0; i<n-prd_int-1; i++) {
        float y = (1 - prd_frac) * x[i+prd_int] + prd_frac * x[i+prd_int+1];
        sum_pwr += pow(x[i],2) + pow(y,2);
        sum_res += pow(x[i]-y,2);
    }
    
    if(sum_pwr == 0.f)
        return 1.f;
    
    return sum_res / sum_pwr;
}