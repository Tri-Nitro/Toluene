from setuptools import setup

setup(
    name='toluene',
    version='0.0.9',
    description='Python library for geospatial and image processing functions',
    packages=[
        'toluene',
    ],
    test_suite='toluene.test',
    install_requires=[
        'numpy'
    ],
)
