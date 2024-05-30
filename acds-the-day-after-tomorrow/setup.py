try:
    from setuptools import setup, find_packages
except ImportError:
    from distutils.core import setup

setup(
    name="ACDS",
    version="0.1.0",
    packages=find_packages(),
)
