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
from toluene_extensions.models.earth.earth_orientation_parameters import new_EOPTable, add_record

from datetime import datetime


class EarthOrientationTable:

    def __init__(self, path: str = None, capsule=None):
        if capsule is not None:
            self.__eop_table = capsule
        else:
            self.__eop_table = new_EOPTable()
        if path is not None:
            self.load_from_file(path)

    def load_from_file(self, path: str):
        with open(path, 'r') as file:
            for line in file:
                year = int(line[0:2])
                month = int(line[2:4])
                day = int(line[4:6])
                if line[16] != ' ':
                    is_bulletin_a_PM_predicted = line[16] == 'P'
                    bulletin_a_PM_x = float(line[18:27])
                    bulletin_a_PM_x_error = float(line[27:36])
                    bulletin_a_PM_y = float(line[37:46])
                    bulletin_a_PM_y_error = float(line[46:55])
                    is_bulletin_a_dut1_predicted = line[57] == 'P'
                    bulletin_a_dut1 = float(line[58:68])
                    bulletin_a_dut1_error = float(line[68:78])
                    if line[79:86] != '       ':
                        bulletin_a_lod = float(line[79:86])
                        bulletin_a_lod_error = float(line[86:93])
                    else:
                        bulletin_a_lod = 0.0
                        bulletin_a_lod_error = 0.0
                    if line[134:144] != '          ':
                        bulletin_b_PM_x = float(line[134:144])
                        bulletin_b_PM_y = float(line[144:154])
                        bulletin_b_dut1 = float(line[154:165])
                    else:
                        bulletin_b_PM_x = 0.0
                        bulletin_b_PM_y = 0.0
                        bulletin_b_dut1 = 0.0
                else:
                    is_bulletin_a_PM_predicted = False
                    bulletin_a_PM_x = 0.0
                    bulletin_a_PM_x_error = 0.0
                    bulletin_a_PM_y = 0.0
                    bulletin_a_PM_y_error = 0.0
                    is_bulletin_a_dut1_predicted = False
                    bulletin_a_dut1 = 0.0
                    bulletin_a_dut1_error = 0.0
                    bulletin_a_lod = 0.0
                    bulletin_a_lod_error = 0.0
                    bulletin_b_PM_x = 0.0
                    bulletin_b_PM_y = 0.0
                    bulletin_b_dut1 = 0.0
                if year <= 73:
                    year += 2000
                else:
                    year += 1900
                timestamp = datetime(year, month, day, 0, 0, 0, 0).timestamp()
                add_record(self.__eop_table, timestamp, is_bulletin_a_PM_predicted, bulletin_a_PM_x,
                           bulletin_a_PM_x_error, bulletin_a_PM_y, bulletin_a_PM_y_error,
                           is_bulletin_a_dut1_predicted, bulletin_a_dut1, bulletin_a_dut1_error, bulletin_a_lod,
                           bulletin_a_lod_error, bulletin_b_PM_x, bulletin_b_PM_y, bulletin_b_dut1)

    @property
    def capsule(self):
        return self.__eop_table
