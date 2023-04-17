import logging

from toluene.util.exception import ImplementationError

logger = logging.getLogger('toluene.compression.codec')


class Codec:
    """
    Instance of codec for data encoding and decoding
    """
    def __init__(self):
        logger.debug(f'Initializing Codec()')
        logger.debug(f'Finished Initializing Codec')

    def decode(self, data: bytes) -> bytes:
        """
        Pure virtual method for decoding bytes

        Args:
            :param data: The data being decoded

        Raises:
            ``toluene.util.exception.ImplementationError``

        Returns:
            :return: ``bytes`` of the decoded data
        """

        logger.debug(f'Entering Codec.decode(bytes of length - {len(bytes)})')

        raise ImplementationError()
