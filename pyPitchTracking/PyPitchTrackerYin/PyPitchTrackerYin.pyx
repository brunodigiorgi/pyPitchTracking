import cython
import numpy as np
cimport numpy as np
from libcpp cimport bool


cdef extern from "PitchTracking/include/PitchTracker.h":
    cdef cppclass PitchTrackerYin:
        PitchTrackerYin(float sample_rate, float max_freq, float thresh, float min_weight) except +
        void setSampleRate(float sample_rate)
        void setMaxFreq(float max_freq)
        void process(float*x, int n, float *p, float *a)
        void process_prob(float*x, int n, float **out_p, float **out_prob, int *out_n) 


cdef class PyPitchTrackerYin:
    cdef PitchTrackerYin *thisptr      # hold a C++ instance which we're wrapping

    def __cinit__(self, float sample_rate=44100.0, float max_freq=1500.0, float thresh=0.1, float min_weight=0.01):
        """
        Parameters
        ----------
        thresh : float
            used for process()
        min_weight : float
            used for process_prob()
        """
        self.thisptr = new PitchTrackerYin(sample_rate, max_freq, thresh, min_weight)

    def __dealloc__(self):
        del self.thisptr

    def setSampleRate(self, sample_rate):
        self.thisptr.setSampleRate(sample_rate)

    def setMaxFreq(self, max_freq):
        self.thisptr.setMaxFreq(max_freq)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def process(self, np.ndarray[float, ndim=1, mode="c"] input not None):
        """
        set the pts list within which to find the path

        Parameters
        ----------
        array : numpy.ndarray[float, ndim=1]
        """
        cdef int n
        cdef float p, a
        n = input.shape[0]

        self.thisptr.process(&input[0], n, &p, &a)
        return p, a

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def process_prob(self, np.ndarray[float, ndim=1, mode="c"] input not None):
        cdef int n
        n = input.shape[0]
        cdef float* out_per;
        cdef float* out_prob;
        cdef int out_n;

        self.thisptr.process_prob(&input[0], n, &out_per, &out_prob, &out_n)
        out_per_ndarray = np.zeros(out_n, dtype=float)
        out_prob_ndarray = np.zeros(out_n, dtype=float)
        for i in range(out_n):
            out_per_ndarray[i] = out_per[i]
            out_prob_ndarray[i] = out_prob[i]
        assert(len(out_per_ndarray) == len(out_prob_ndarray))
        return out_per_ndarray, out_prob_ndarray
