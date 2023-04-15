import logging

logger = logging.getLogger('toluene.image.image')


class Image:
    """
    Defines pixel data formats from files

    Args:
        file (str): The file being read in.
    """
    def __init__(self, file: str = None):

        logger.debug(f'Initializing Image({file})')

        self._image_data = None
        if file is not None:
            self.open(file)

        logger.debug(f'Finished Initializing Image')

    def open(self, file: str):

        logger.debug(f'Entering Image.open({file})')

        with open(file, mode='rb') as image_file:
            self._image_data = image_file.read()
        self._parse()

    def _parse(self):
        raise NotImplementedError()
