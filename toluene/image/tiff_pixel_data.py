from toluene.image.pixel_data import PixelData


class TIFFPixelData(PixelData):
    """
    Defines pixel data for TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
    """

    def __init__(self, image_ifd: dict, image_data: bytes):
        super().__init__(image_data)

        self._compressions = image_ifd['Compression']
