from toluene.image.image import Image
from toluene.util.exception import MagicNumberError


class TIFF(Image):
    def __init__(self, file: str = None):
        super().__init__(file)

    class IFD:

        def __init__(self, header: bytes, byte_order: str):
            self.__tag = int.from_bytes(header[:2], byte_order)
            self.__field_type = int.from_bytes(header[2:4], byte_order)
            self.__count = int.from_bytes(header[4:8], byte_order)
            self.__offset = int.from_bytes(header[8:], byte_order)

        def __str__(self):
            return f'IFD<tag:={self.__tag},field_type:={self.__field_type},' \
                   f'count:={self.__count},offset:={self.__offset}>'

    def _parse(self):

        # Determine the byteorder
        byte_order_flag = self._image_file[:2]
        if byte_order_flag == b'II':
            self._byte_order = "little"
        elif byte_order_flag == b'MM':
            self._byte_order = "big"

        if int.from_bytes(self._image_file[2:4], self._byte_order) != 42:
            raise MagicNumberError()

        self._ifd_offset = int.from_bytes(self._image_file[4:8], self._byte_order)

        print(TIFF.IFD(self._image_file[self._ifd_offset:self._ifd_offset + 12], self._byte_order))
