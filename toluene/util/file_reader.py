import os
from zipfile import ZipFile, is_zipfile

from typing import List

from toluene.util.exception import ReadError


class Directory:
    """
    Abstract Directory for reading compressed and uncompressed dirs agnostically
    """

    def __init__(self, path: str):
        self.__path = path

    def __getitem__(self, item: str):
        return get_file(self.__path, item)

    def listdir(self) -> List[str]:
        return listdir(self.__path)


def listdir(path: str) -> List[str]:
    if is_zipfile(path):
        with ZipFile(path) as zipfile:
            return zipfile.namelist()
    elif os.path.isdir(path):
        return os.listdir(path)


def get_file(path: str, file_or_dirname: str):
    if is_zipfile(path):
        with ZipFile(path) as zipfile:
            return zipfile.extract(file_or_dirname)
    elif os.path.isdir(path):
        pass
    else:
        raise ReadError()
