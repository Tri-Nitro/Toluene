from datetime import datetime, timezone

# Exact time of J2000.0 in utc
unix_j2000_time_offset = datetime(2000, 1, 1, 11, 58, 55, 816000, tzinfo=timezone.utc).timestamp()

# List of leap seconds after J2000.0 TODO: make this configurable at runtime
leap_seconds_after_j2000 = [1136054400, 1230768000, 1341091200, 1435670400, 1483228800]


class TerrestrialTimeJ2000:
    def __init__(self, dt: datetime = datetime.utcnow()):
        self.__timestamp = dt.timestamp()
        for leap_second in leap_seconds_after_j2000:
            if self.__timestamp >= leap_second:
                self.__timestamp += 1
        self.__timestamp -= unix_j2000_time_offset

    def seconds_since(self) -> float:
        return self.__timestamp

    def centuries_since(self) -> float:
        return self.__timestamp / 36525.0

    def __str__(self):
        timestamp = self.__timestamp % 86400
        hh = int(timestamp // 3600)
        mm = int(timestamp // 60 % 60)
        ss = int(timestamp % 60)
        return '{:02d}:{:02d}:{:02d}'.format(hh, mm, ss)