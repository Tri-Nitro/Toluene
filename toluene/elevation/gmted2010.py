import logging

from toluene.util.file_reader import Directory

logger = logging.getLogger('toluene.elevation.gmted2010')

class GMTED2010:
    """
    Defines the Global Multi-resolution Terrain Elevation Data 2010

    Args:
        :param data_path: The path where the data is installed.
            Defaults to ``'/data'``.
    """

    def __init__(self, data_path: str = '/data'):
        logger.debug(f'Initializing GMTED2010({data_path})')
        self.__data_path = Directory(data_path)
        logger.debug(f'Finished Initializing GMTED2010()')
