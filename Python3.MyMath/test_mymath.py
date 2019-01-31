# from setuptools import setup, Extension
#
# setup(name = "mymath",
#      version = "0.1",
#      ext_modules = [Extension("mymath", ["mymath.cc"])]
#    );
import setup

import os
import sys

# export PYTHONPATH=build/lib.linux-x86_64-3.6
os.environ['PYTHONPATH'] = "build/lib.linux-x86_64-3.6"
import mymath
