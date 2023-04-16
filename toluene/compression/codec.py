import numpy as np

from toluene.util.exception import ImplementationError


class Codec:
    def __init__(self):
        pass

    def decode(self, data: bytes) -> np.array:
        raise ImplementationError()
