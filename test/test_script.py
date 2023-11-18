from datetime import datetime

from toluene.base.datetime import JulianDateTime

jd = JulianDateTime(datetime.now())

print(jd.gmst)