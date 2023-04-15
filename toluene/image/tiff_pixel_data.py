from typing import List

from toluene.image.pixel_data import PixelData


class TiffPixelData(PixelData):

    def __init__(self, image_ifd: List[dict], image_data: bytes):
        self._compression = [ifd['Compression'] for ifd in image_ifd]
        print(self._compression)
        super().__init__(image_data)
