from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy

setup(
    name='pyPitchTracking',
    ext_modules = cythonize([
        Extension("pyPitchTracking", 
                  sources=["pyPitchTracking.pyx", 
                           "PitchTracking/src/PitchTracker.cpp",
                           "PitchTracking/src/utils.cpp",
                           ], 
                  include_dirs=['PitchTracking/include', numpy.get_include()],
                  extra_compile_args=['-std=gnu++11', '-stdlib=libc++', '-mmacosx-version-min=10.7'],
                  extra_link_args=['-mmacosx-version-min=10.7'],
                  language='c++',
                  )
        ])
)