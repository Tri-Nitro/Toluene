def is_opencl_available():
    try:
        import toluene_extensions.opencl.context
    except ImportError:
        return False
    return True