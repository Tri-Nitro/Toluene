class CLibraryNotFound(Exception):
    """
    Used when unable to find a C-Library during the loading process
    """
    pass


class ReadError(Exception):
    """
    Used when unable to read a file, dir, or archive
    """
    pass


class LatitudeOutOfRange(Exception):
    """
    Used when Latitude is out of range, I.E. -90 <= latitude <= 90
    """
    pass
