import logging
import zlib

from toluene.compression.codec import Codec

logger = logging.getLogger('toluene.compression.deflate')


class Deflate(Codec):
    """
    Codec for Adobe deflate codec algorithm.
    """

    def __init__(self):
        logger.debug(f'Initializing Deflate()')
        super().__init__()
        logger.debug(f'Finished Initializing Deflate()')

    def decode(self, data: bytes) -> bytes:
        """
        Inflates an Adobe Deflated stream.

        Args:
            data (bytes): The deflated input data.

        Returns:
            bytes of the inflated data.
        """

        logger.debug(f'Entering Deflate.decode(bytes of length - {len(bytes)})')

        decompress = zlib.decompressobj(
            zlib.MAX_WBITS
        )
        inflated = decompress.decompress(data)
        inflated += decompress.flush()

        return inflated


"""
Singleton of the Adobe Deflate codec.
"""
deflate_compression = Deflate()
