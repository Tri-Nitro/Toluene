from setuptools import setup


def readme():
    with open('README.md') as f:
        return f.read()


setup(
    name='toluene',
    version='0.0.19',
    license='MIT',
    description='Python library for geospatial and image processing functions',
    long_description=readme(),
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'License :: OSI Approved :: MIT License',
        'Operating System :: Microsoft',
        'Operating System :: POSIX',
        'Programming Language :: C',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Topic :: Scientific/Engineering :: GIS',
        'Topic :: Scientific/Engineering :: Image Processing',
        'Typing :: Typed'
    ],
    packages=[
        'toluene',
    ],
    test_suite='toluene.test',
    install_requires=[
        'numpy'
    ],
)
