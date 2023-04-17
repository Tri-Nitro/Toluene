from ctypes import RTLD_LOCAL, CDLL
import os


class CLibrary:
    def __init__(self, library_name: str = None):
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

    def find_library(self, hint_path: str, library_type: str = 'dynamic') -> bool:
        """
        Finds and loads the library specified in the location given.

        Args:
            hint_path (str): the string of either ; in windows or : linux separated locations to search for the lib
            library_type (str): the library type being searched for. Defaults to dynamic.

        Returns:
            A True if the library was able to located and installed.
        """
        if library_type == 'dynamic':
            library_extension = self.__dynamic_library_extension
        else:
            library_extension = self.__static_library_extension

        given_dirs = hint_path.split(self.__file_separator)
        for hint in given_dirs:
            if os.path.exists(hint):
                for file in os.listdir(hint):
                    if not os.path.isdir(file):
                        if file.startswith(f'lib{self.__library_name}{library_extension}'):
                            self.__found_library = True
                            self.__library_path = f'{os.path.join(hint, file)}'
                            self.__load_library()
                            return True

        return False

    def __load_library(self, mode=RTLD_LOCAL):
        self.library = CDLL(self.__library_path, mode=mode)

    def found_library(self) -> True:
        """
        Tells if the library was found or not

        Returns:
             True if the library was found
        """
        return self.__found_library

    def library_path(self) -> str:
        """
        Tells the path the library was found at.

        Returns:
            The full path to the library
        """
        return self.__library_path
