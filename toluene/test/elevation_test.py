import unittest

from toluene.elevation.gmted2010 import GMTED2010


class GMTED2010TestCase(unittest.TestCase):
    def test_loading_dataset(self):
        data = GMTED2010("E:\\GMTED2010\\GMTED2010N10E000_075.zip")
        self.assertEqual(True, True)  # add assertion here


if __name__ == '__main__':
    unittest.main()
