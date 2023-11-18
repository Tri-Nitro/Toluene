from datetime import datetime, timedelta

from toluene_extensions.base_extensions import greenwich_mean_sidereal_time

# Exact time of J2000 in utc
unix_julian_time_offset = datetime(2000, 1, 1, 11, 58, 55, 816000)
# Number of seconds in Julian Century without leap seconds added
seconds_in_julian_century = 3155760000.0
seconds_in_day = 86400.0
seconds_in_hour = 3600.0
hours_in_day = 24.0


class JulianDateTime:

    def __init__(self, timestamp):
        self.__timestamp = timestamp
        self.__time_jd = self.__timestamp - unix_julian_time_offset
        self.__century = self.__time_jd.total_seconds() / seconds_in_julian_century
        self.__day_jd = self.__timestamp.replace(hour=0, minute=0, second=0, microsecond=0)
        self.gmst = self._greenwich_mean_sidereal_time()
        self.gast = self._greenwich_apparent_sidereal_time()

    def time(self) -> timedelta:
        return self.__time_jd

    def century(self) -> float:
        return self.__century

    def _greenwich_mean_sidereal_time(self) -> float:
        DUT = self.__time_jd.total_seconds()
        H = (self.__timestamp - self.__day_jd).total_seconds()
        return greenwich_mean_sidereal_time(DUT, H, self.__century)

    def _greenwich_apparent_sidereal_time(self) -> float:
        equation_of_equinoxes = 0.0
        return self.gmst + equation_of_equinoxes
