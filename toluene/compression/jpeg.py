import logging

from toluene.compression.codec import Codec

logger = logging.getLogger('toluene.compression.jpeg')


class JPEG(Codec):
    """
    JPEG codec to decode and encode jpeg data.
    """

    def __init__(self):
        logger.debug(f'Initializing JPEG()')
        super().__init__()
        logger.debug(f'Finished Initializing JPEG()')

    def decode(self, data: bytes) -> bytes:
        """
        Decodes JPEG data

        Args:
            :param data: The jpeg encoded input data.

        Returns:
            :return: ``bytes`` of the decoded input.
        """

        logger.debug(f'Entering JPEG.decode()')

        return data