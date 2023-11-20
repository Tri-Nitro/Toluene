from datetime import datetime, timezone

from toluene.base.coordinates import ECEF
from toluene.base.terrestrial_time import TerrestrialTimeJ2000

from toluene_extensions.base_extensions import ecef_from_eci

TT = TerrestrialTimeJ2000(datetime(2023, 11, 20, 0, 0, 0, tzinfo=timezone.utc))

ecef = ECEF(50000, 50000, 50000)
print(ecef_from_eci(ecef.x, ecef.y, ecef.z, TT.seconds_since()))
