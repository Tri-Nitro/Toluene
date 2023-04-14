from toluene.util.file_reader import Directory


class GMTED2010:
    """
    Defines the Global Multi-resolution Terrain Elevation Data 2010

    Args:
        data_path (str): The path where the data is installed. Defaults to /data.
    """

    def __init__(self, data_path: str = '/data'):
        self.__data_path = Directory(data_path)
