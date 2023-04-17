import logging

logger = logging.getLogger('toluene.image.image')


class Image:
    """
    Defines pixel data formats from files

    Args:
        :param file: The file being read in.
    """
    def __init__(self, file: str = None):

        logger.debug(f'Initializing Image({file})')

        self._image_file = None
        self._pixel_data = None

        if file is not None:
            self.open(file)

        logger.debug(f'Finished Initializing Image')

    def open(self, file: str):
        """
        Opens an image file and parses it using the subclass parser.

        Args:
            :param file: The input file.
        """

        logger.debug(f'Entering Image.open({file})')

        with open(file, mode='rb') as image_file:
            self._image_file = image_file.read()
        self._parse()

    def _parse(self):
        """
        Pure Virtual method for parsing an image stream or file.

        Raises:
            ``NotImplementedError``
        """
        raise NotImplementedError()
