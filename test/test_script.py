from datetime import datetime, timezone

from toluene.core.coordinates import ECEF
from toluene.core.time import TerrestrialTimeJ2000


TT = TerrestrialTimeJ2000(datetime(2023, 11, 20, 0, 0, 0, tzinfo=timezone.utc))

ecef = ECEF(50000, 50000, 50000, time=TT)
print(ecef.magnitude(), ecef.to_eci().magnitude())
