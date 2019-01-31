from setuptools import setup, Extension

setup(name = "mymath",
        version = "0.1",
        ext_modules = [Extension("mymath", ["mymath.cc"])]
        );
