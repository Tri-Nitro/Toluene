class PixelData:
    """
    Defines pixel data for images

    Args:
        image_data (bytes): The bytes of the image file or image stream.

    """
    def __init__(self, image_data: bytes):

        self._image_width = 0
        self._image_length = 0
        self._bit_depth = 0
        self._color_depth = 0
