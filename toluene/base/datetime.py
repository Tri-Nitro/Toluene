import time
from datetime import datetime, timedelta

# Exact time of J2000 in utc
unix_julian_time_offset = datetime(2000, 1, 1, 11, 58, 55, 816000)
# Number of seconds in Julian Century without leap seconds added
seconds_in_julian_century = 3155760000


class JulianDateTime:

    def __init__(self, timestamp):
        self._timestamp = timestamp

    def time(self) -> timedelta:
        return self._timestamp - unix_julian_time_offset

    def century(self) -> float:
        return self.time().total_seconds() / 3155760000.0
