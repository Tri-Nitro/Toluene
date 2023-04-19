import logging

from toluene.compression.codec import Codec

logger = logging.getLogger('toluene.compression.mock')


class Mock(Codec):
    """
    Fake codec to allow uncompressed data to pass through a ``Codec``
    """

    def __init__(self):
        logger.debug(f'Initializing Mock()')
        super().__init__()
        logger.debug(f'Finished Initializing Mock()')

    def decode(self, data: bytes) -> bytes:
        """
        No real compression but is in place to pretend to be decompressing
        data that assumes a codec in place

        Args:
            :param data: The deflated input data.

        Returns:
            :return: ``bytes`` of the inflated data.
        """

        logger.debug(f'Entering Mock.decode()')

        return data


"""
Singleton of the Mock codec.
"""
no_compression = Mock()
