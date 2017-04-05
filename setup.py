from setuptools import find_packages
from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy

setup(
    name='pyPitchTracking',
    version='0.1',
    description='Python Pitch Tracking',
    author='Bruno Di Giorgi',
    author_email='bruno@brunodigiorgi.it',
    url='https://github.com/brunodigiorgi/pyPitchTracking',
    license="GPLv2",
    packages=find_packages(),
    include_package_data=False,
    zip_safe=False,
    install_requires=[],
    extras_require={},
    ext_modules=cythonize([
        Extension("pyPitchTracking.PyPitchTrackerYin",
                  sources=["pyPitchTracking/PyPitchTrackerYin/PyPitchTrackerYin.pyx",
                           "pyPitchTracking/PyPitchTrackerYin/PitchTracking/src/PitchTracker.cpp",
                           "pyPitchTracking/PyPitchTrackerYin/PitchTracking/src/utils.cpp",
                           ],
                  include_dirs=['pyPitchTracking/PyPitchTrackerYin/PitchTracking/include', numpy.get_include()],
                  extra_compile_args=['-std=gnu++11', '-stdlib=libc++', '-mmacosx-version-min=10.7'],
                  extra_link_args=['-mmacosx-version-min=10.7'],
                  language='c++',
                  )
    ])
)
