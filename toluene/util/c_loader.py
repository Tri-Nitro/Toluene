import logging
from ctypes import RTLD_LOCAL, CDLL
import os

logger = logging.getLogger('toluene.util.c_loader')


class CLibrary:
    """
    Locates and loads in C Libraries for use with python.

    Args:
        :param library_name: The name of the library.
            example ``libtoluene.so`` is ``'toluene'``
    """
    def __init__(self, library_name: str = None):
        logger.debug(f'Initializing CLibrary({library_name})')

        self.__library_name = library_name

        self.__found_library = False
        self.__library_path = None

        self.library = None

        if os.name == 'nt':
            self.__dynamic_library_extension = '.dll'
            self.__static_library_extension = '.a'
            self.__install_path = os.environ['PATH']
            self.__file_separator = ';'
        elif os.name == 'posix':
            self.__dynamic_library_extension = '.so'
            self.__static_library_extension = '.a'
            try:
                self.__install_path = os.environ['LD_LIBRARY_PATH']
            except KeyError:
                self.__install_path = ''
            self.__file_separator = ':'

        if library_name is not None:
            self.find_library(hint_path=self.__install_path)

        logger.debug(f'Finished Initializing CLibrary')

    def find_library(self, hint_path: str, library_type: str = 'dynamic')\
            -> bool:
        """
        Finds and loads the library specified in the location given.

        Args:
            :param hint_path: the string of either ; in windows or : linux
                separated locations to search for the lib
            :param library_type: the library type being searched for.
                Defaults to ``'dynamic'``.

        Returns:
            :return: A True if the library was able to located and installed.
        """

        logger.debug(f'Entering CLibrary.find_library({hint_path}, '
                     f'{library_type})')

        if library_type == 'dynamic':
            library_extension = self.__dynamic_library_extension
        else:
            library_extension = self.__static_library_extension

        given_dirs = hint_path.split(self.__file_separator)
        for hint in given_dirs:
            if os.path.exists(hint):
                for file in os.listdir(hint):
                    if not os.path.isdir(file):
                        if file.startswith(f'lib{self.__library_name}'
                                           f'{library_extension}'):
                            self.__found_library = True
                            self.__library_path = f'{os.path.join(hint, file)}'
                            self.__load_library()
                            return True

        return False

    def __load_library(self, mode=RTLD_LOCAL):
        """
        Loads the library from the library file into the class.

        Args:
            :param mode: The mode the library will be loaded in with.
                (Defaults to ctypes.RTLD_LOCAL)
        """
        logger.debug(f'Entering CLibrary.__load_library({mode})')
        self.library = CDLL(self.__library_path, mode=mode)

    def found_library(self) -> True:
        """
        Tells if the library was found or not.

        Returns:
             :return: ``True`` if the library was found.
        """

        return self.__found_library

    def library_path(self) -> str:
        """
        Tells the path the library was found at.

        Returns:
            :return: The full path to the library.
        """
        return self.__library_path