class Image:
    def __init__(self, file: str = None):
        self._image_file = None
        if file is not None:
            self.open(file)

    def open(self, file: str):
        with open(file, mode='rb') as image_file:
            self._image_file = image_file.read()
        self._parse()

    def _parse(self):
        raise NotImplementedError()
