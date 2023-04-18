from threading import Thread
from typing import List, Literal

from toluene.compression.codec import Codec


class MultiThreadedCodecRunner:
    """
    Runs multiple codecs in parallel to increase decoding and encoding speed.

    Args:
        :param codec: The codec for the runner to use.
        :param data: The list of bytes to encode or decode.
        :param num_threads: The max number of threads to execute at a time.
    """

    def __init__(self, codec: Codec, data: List[bytes],
                 mode: Literal["encode", "decode"], num_threads: int = 8):
        self.__codec = codec
        self.__data = data
        self.__mode = mode

        self.__threads = [CodecThread(self.__codec, thread_data, self.__mode)
                          for thread_data in self.__data]

    def run(self) -> List[bytes]:
        """
        Runs the codec on each of the data blocks and returns their values
        in a list.

        Returns:
            :return: List of encoded or decoded data blocks.
        """
        [thread.start() for thread in self.__threads]
        [thread.join() for thread in self.__threads]
        return [thread.value for thread in self.__threads]


class CodecThread(Thread):
    """
    A threaded Codec encoder or decoder.

    Args:
        :param codec: The codec to use in the thread.
        :param data: The data to be encoded or decoded.
        :param mode: Either to encode or decode the data.
    """

    def __init__(self, codec: Codec, data: bytes, mode: Literal["encode", "decode"]):

        Thread.__init__(self)

        self.value = None

        self.__codec = codec
        self.__data = data
        self.__mode = mode

    def run(self) -> None:
        if self.__mode == 'encode':
            self.value = self.__codec.encode(data=self.__data)
        elif self.__mode == 'decode':
            self.value = self.__codec.decode(data=self.__data)
