from toluene.opencl.context import OpenCLContext
from toluene_extensions.opencl import context

class OpenCLKernel:
    """
    A kernel is a function that can be executed on a device.
    """
    def __init__(self, py_context: OpenCLContext, name: str, source_file: str):
        with open(source_file, 'r') as file:
            source = file.read()
            self.__kernel = context.new_opencl_kernel(py_context.capsule, source, name)

    @property
    def capsule(self):
        return self.__kernel
