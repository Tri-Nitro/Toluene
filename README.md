# Toluene
Python library for geospatial and image processing functions.

## What is Toluene?

Toluene is a library writen in Python with C acceleration. It delivers fast
python functions for anything you could need when doing geospatial and/or image
processing. It aims to make installing a breeze for both online and offline
installs. It favors rebuilding the wheel over the use of propitiatory software
like Kakadu to maintain the accessibility and ease of use of the library.

## Installing Toluene

Toluene being a C accelerated python library you need to build from source the 
C library. CMake is used so it should be as simple as

```
cmake <toluene_clone_path>
make
```

in UNIX systems and in Windows using the CMake GUI to build to a VisualStudio
target. Once the library is build all you have to do is add the library dir to
your systems search path `PATH` in Windows or `LD_LIBRARY_PATH` on UNIX systems.

## Documentation

Documentation can be found on our read-the-docs page at [https://nitro-toluene.org](https://nitro-toluene.org)