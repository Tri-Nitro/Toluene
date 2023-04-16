import zlib

from toluene.compression.codec import Codec


class Deflate(Codec):

    def __init__(self):
        super().__init__()

    def decode(self, data: bytes) -> bytes:

        decompress = zlib.decompressobj(
            zlib.MAX_WBITS
        )
        inflated = decompress.decompress(data)
        inflated += decompress.flush()

        return inflated


deflate_compression = Deflate()
