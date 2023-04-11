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
        elif os.name == 'posix':
            self.__dynamic_library_extension = '.so'
            self.__static_library_extension = '.a'
            self.__install_path = os.environ['LD_LIBRARY_PATH']

        if library_name is not None:
            self.find_library(hint_path=self.__install_path)

    def find_library(self, hint_path: str = None, library_type: str = 'dynamic') -> bool:

        if library_type == 'dynamic':
            library_extension = self.__dynamic_library_extension
        else:
            library_extension = self.__static_library_extension

        if hint_path is not None:
            given_dirs = hint_path.split(':')
            for hint in given_dirs:
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
        return self.__found_library

    def library_path(self) -> str:
        return self.__library_path
