# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                   #
#   MIT License                                                                     #
#                                                                                   #
#   Copyright (c) 2023 Tri-Nitro                                                    #
#                                                                                   #
#   Permission is hereby granted, free of charge, to any person obtaining a copy    #
#   of this software and associated documentation files (the "Software"), to deal   #
#   in the Software without restriction, including without limitation the rights    #
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       #
#   copies of the Software, and to permit persons to whom the Software is           #
#   furnished to do so, subject to the following conditions:                        #
#                                                                                   #
#   The above copyright notice and this permission notice shall be included in all  #
#   copies or substantial portions of the Software.                                 #
#                                                                                   #
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#   SOFTWARE.                                                                       #
#                                                                                   #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
import yaml

from toluene.models.earth.earth_orientation_table import EarthOrientationTable
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.nutation import NutationSeries
from toluene.util.file import configdir, datadir
from toluene_extensions.models.earth import earth

# Default is set to the WGS84 ellipsoid.
# This is global and can be overwritten with by redefining this tuple to the desired semi-major and semi-minor axes.
default_ellipsoid = (6378137.0, 6356752.314245)


class EarthModel:
    """
    Class to represent a model of the Earth. This is not stored in python but in C. All computation using the classes
    are just wrappers for C functions. Uses the defaults that ship with the library. As the library becomes outdated,
    it is strongly advised to use your own data. Nutation Series should be very accurate for the next 100 years, but
    the Earth Orientation Table is only accurate for the dates it was packaged. The default is the finals2000A.all
    file which is pulled from the IERS website. The file is included in the package, but it is recommended to download
    the latest file from the IERS website and set the eop_table to that file.

    :param ellipsoid: The ellipsoid model to use for the Earth.
    :type ellipsoid: :class:`toluene.models.earth.Ellipsoid`
    :param nutation_series: The nutation series to use for the Earth. Giving a nutation series will override the
        default nutation series. The Earth Model will take ownership of the nutation series and will delete the series
        passed in so all references to the series should go through the model.
    :type nutation_series: :class:`toluene.models.earth.NutationSeries`
    :param eop_table: The Earth Orientation Table to use for the Earth. Giving an Earth Orientation Table will
        override the default Earth Orientation Table. The Earth Model will take ownership of the Earth Orientation
        Table and will delete the table passed in so all references to the table should go through the model.
    :type eop_table: :class:`toluene.models.earth.EarthOrientationTable`
    """
    def __init__(self, ellipsoid: Ellipsoid = None, nutation_series: NutationSeries = None,
                 eop_table: EarthOrientationTable = None, capsule=None):
        if capsule is None:
            self.__model = earth.new_EarthModel()

            if ellipsoid is None:
                earth.set_ellipsoid(self.__model, default_ellipsoid[0], default_ellipsoid[1])
            else:
                earth.set_ellipsoid(self.__model, ellipsoid.semi_major_axis, ellipsoid.semi_minor_axis)

            if nutation_series is None:
                nutation_series = NutationSeries()
                with open(configdir + '/nutation.yml') as f:
                    yaml_config = yaml.safe_load(f)
                nutation_series.load_from_list(yaml_config['nutation']['series'])
            earth.set_nutation_series(self.__model, nutation_series.capsule)

            if eop_table is None:
                eop_table = EarthOrientationTable()
                eop_table.load_from_file(datadir + '/finals2000A.all')
            earth.set_earth_orientation_parameters(self.__model, eop_table.capsule)

    """
    Gets the model and returns it as a capsule.
    """
    @property
    def capsule(self):
        return self.__model
