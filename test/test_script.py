from datetime import datetime, timezone

from toluene.core.coordinates import ECEF
from toluene.core.time import TerrestrialTimeJ2000


TT = TerrestrialTimeJ2000(datetime(2023, 11, 20, 0, 0, 0, tzinfo=timezone.utc))

ecef = ECEF(50000, 50000, 50000, time=TT)
eci = ecef.to_eci()
print(ecef.x, ecef.y, ecef.z)
print(eci.x, eci.y, eci.z)
print(ecef.magnitude(), eci.magnitude())
