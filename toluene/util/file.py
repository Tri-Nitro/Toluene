import os
import uuid

"""
Temp dir for this run of toluene. It uses a uuid to make sure that there are no collisions with other runs of toluene.
"""
tempdir = None

if tempdir is None:
    if os.name == 'nt':
        tempdir = os.environ['TEMP'] + f'\\toluene-{uuid.uuid4()}'
    else:
        tempdir = f'/tmp/toluene-{uuid.uuid4()}'

    os.path.exists(tempdir) or os.makedirs(tempdir)
