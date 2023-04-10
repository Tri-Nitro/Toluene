from setuptools import setup

setup(
    name='toluene',
    version='0.0.0',
    description='Python library for geospatial and image processing functions',
    packages=[
        'toluene.base',
        'toluene.test',
    ],
    test_suite='toluene.test',
)
