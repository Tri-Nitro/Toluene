import os
import uuid

"""
Temp dir for this run of toluene. It uses a uuid to make sure that there are no collisions with other runs of toluene.
"""
tempdir = None
datadir = os.path.dirname(os.path.realpath(__file__)) + '/../data/'
configdir = os.path.dirname(os.path.realpath(__file__)) + '/../config/'
kerneldir = os.path.dirname(os.path.realpath(__file__)) + '/../kernels/'

if tempdir is None:
    if os.name == 'nt':
        tempdir = os.environ['TEMP'] + f'\\toluene-{uuid.uuid4()}'
    else:
        tempdir = f'/tmp/toluene-{uuid.uuid4()}'

    os.path.exists(tempdir) or os.makedirs(tempdir)
