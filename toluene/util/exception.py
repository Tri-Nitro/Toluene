class CLibraryNotFound(Exception):
    """
    Used when unable to find a C-Library during the loading process
    """
    pass


class ImplementationError(Exception):
    """
    Used when a pure virtual function was not implemented.
    """
    pass


class JPEGDecodingError(Exception):
    """
    Used when an error occurs decoding a JPEG.
    """
    pass


class LatitudeOutOfRange(Exception):
    """
    Used when Latitude is out of range, I.E. -90 <= latitude <= 90.
    """
    pass


class MagicNumberError(Exception):
    """
    Used when magic number in file doesn't match the specification.
    """
    pass


class ReadError(Exception):
    """
    Used when unable to read a file, dir, or archive
    """
    pass


class UndefinedTagError(Exception):
    """
    Used when a bad TIFF tag is encountered.
    """
    pass