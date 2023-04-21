import logging

from toluene.compression.codec import Codec
from toluene.util.exception import JPEGDecodingError

logger = logging.getLogger('toluene.compression.jpeg')

jpeg_markers = {
    0xffc0: 'start_frame',
    0xffc4: 'huffman_table',
    0xffd8: 'start',
    0xffd9: 'end',
    0xffda: 'scan_start',
    0xffdb: 'quantization_table',
    0xffe0: 'app_default_header',
}


class JPEG(Codec):
    """
    JPEG codec to decode and encode jpeg data.

    Args:
        :param jpeg_tables: additional tables to use on all jpegs decoded by
            jpeg codec.
    """

    def __init__(self, jpeg_tables: bytes = None):

        logger.debug(f'Initializing JPEG()')
        super().__init__()

        self.quantization_table = []

        if jpeg_tables is not None:
            self.__parse(jpeg_tables)

        logger.debug(f'Finished Initializing JPEG()')

    def __parse(self, jpeg_table: bytes):

        pointer = 0
        while pointer < len(jpeg_table):
            marker = jpeg_table[pointer:pointer + 2]
            # Start of jpeg. Useless flag.
            if marker == b'\xff\xd8':
                pointer += 2
                continue
            # End of jpeg. Exit if at end.
            elif marker == b'\xff\xd9':
                pointer += 2
                if pointer != len(jpeg_table):
                    raise JPEGDecodingError()
                continue
            elif marker == b'\xff\xdb':
                pointer += 2
                length = int.from_bytes(jpeg_table[pointer:pointer+2],
                                        byteorder='little')
                pointer += 2
                self.quantization_table.append(QuantizationTable(
                    jpeg_table[pointer:pointer+length-2]))
                pointer += length-2
            else:
                print(marker)
                raise JPEGDecodingError()

    def decode(self, data: bytes) -> bytes:
        """
        Decodes JPEG data

        Args:
            :param data: The jpeg encoded input data.

        Returns:
            :return: ``bytes`` of the decoded input.
        """

        logger.debug(f'Entering JPEG.decode()')

        print(data)

        return data


class QuantizationTable:
    """
    Defines a JPEG Quantization Table.

    Args:
        :param table: The bytes of the table.
    """

    def __init__(self, table: bytes):
        self.__read(table)

    def __read(self, table: bytes):
        """
        Reads in the quantization table from bytes.

        Args:
            :param table: The bytes of the table.
        """
        print(table)
        pass
