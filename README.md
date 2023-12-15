# TOLUENE
___________________

## Description
Orbital mechanics and Geospatial toolkit library for Python accelerated in C and OpenCL.
# STILL A WORK IN PROGRESS AND NOT READY FOR PRODUCTION USE

## Installation
No OpenCL support installs can be done with pip.
```bash
pip install toluene
```
Toluene Doesn't support Conda. The purpose is for production level code. Anaconda is a heavy unnessary dependency.
Coming from a C++ and Python background with DevOps experience, I have found that Anaconda is a heavy dependency that
is not needed. I have found that it is better to use a virtual environment and install the dependencies that you need.
Anaconda is a great tool for beginners, but it is not suitable for operations.

For the OpenCL accelerated version, you will need to install the OpenCL drivers for your GPU. For Nvidia, you will need
to install the CUDA toolkit. For AMD, you will need to install the ROCm toolkit. It's easiest to install from source
for toluene with OpenCL enabled. You will need to install the OpenCL headers and libraries.
```bash
git clone https://github.com/Tri-Nitro/Toluene.git
cd Toluene
pip install .
```

You should be able to see it acknowledge the OpenCL library. If you don't see it, then you will need to ensure that
`cl.h` is in your include path and `OpenCL.lib` is in your library path. If you are on Linux, you will need to install
the OpenCL headers and libraries. If you are on Windows, you will need to install the OpenCL headers and libraries from
the Intel SDK or AMD SDK. If you are on Mac, you will need to install the OpenCL headers and libraries from the Intel
SDK. `CUDA_PATH` is used for Nvidia GPUs and `ROCM_PATH` is used for AMD GPUs. To find the path to OpenCL so make sure
those are set approriately and uninstall and install again. This was written by AI so have fun trying to decipher it.