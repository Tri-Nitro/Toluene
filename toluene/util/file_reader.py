from zipfile import ZipFile, is_zipfile


class Directory:
    """
    Abstract Directory for reading compressed and uncompressed dirs agnostically
    """

    def __init__(self, path: str):
        self.__path = path

    def __read(self):
        if is_zipfile(self.__path):
            with ZipFile(self.__path) as zipfile:
                zipfile.read(self.__path)
