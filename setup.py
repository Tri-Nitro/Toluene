import os
import sys

from setuptools import setup, Extension

extensions = [
    Extension(
        'toluene_extensions.coordinates.state_vector',
        [
            'c/src/coordinates/state_vector.c'
        ],
        include_dirs=['c/include']
    ),
    Extension(
        'toluene_extensions.coordinates.transform',
        [
            'c/src/coordinates/state_vector.c',
            'c/src/coordinates/transform.c',
            'c/src/math/constants.c',
            'c/src/math/linear_algebra.c',
            'c/src/models/earth/bias.c',
            'c/src/models/earth/constants.c',
            'c/src/models/earth/earth_orientation_parameters.c',
            'c/src/models/earth/nutation.c',
            'c/src/models/earth/polar_motion.c',
            'c/src/models/earth/precession.c',
            'c/src/models/earth/rotation.c',
            'c/src/models/moon/constants.c',
            'c/src/models/sun/constants.c',
            'c/src/time/constants.c',
            'c/src/time/delta_t.c',
        ],
        include_dirs=['c/include']
    ),
    Extension(
        'toluene_extensions.models.earth.earth',
        [
            'c/src/models/earth/ellipsoid.c',
            'c/src/models/earth/earth.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.earth.geoid',
        [
            'c/src/models/earth/geoid.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.earth.earth_orientation_parameters',
        [
            'c/src/models/earth/earth_orientation_parameters.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.earth.ellipsoid',
        [
            'c/src/models/earth/ellipsoid.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.earth.nutation',
        [
            'c/src/math/constants.c',
            'c/src/models/earth/constants.c',
            'c/src/models/earth/nutation.c',
            'c/src/models/moon/constants.c',
            'c/src/models/sun/constants.c',
            'c/src/time/constants.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.moon.position',
        [
            'c/src/math/constants.c',
            'c/src/models/earth/constants.c',
            'c/src/models/moon/constants.c',
            'c/src/models/moon/position.c',
            'c/src/models/sun/constants.c',
            'c/src/time/constants.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.models.sun.position',
        [
            'c/src/math/constants.c',
            'c/src/models/earth/constants.c',
            'c/src/models/sun/constants.c',
            'c/src/models/sun/position.c',
            'c/src/time/constants.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene_extensions.time.delta_t',
        [
            'c/src/time/delta_t.c',
        ],
        include_dirs=['c/include'],
    ),
]

found_opencl = False
opencl_include_dir = []
opencl_library_dir = []

if 'TOLUENE_OPENCL' in os.environ and os.environ['TOLUENE_OPENCL'] == '1':
    if 'darwin' in sys.platform:
        if 'TOLUENE_OPENCL_INCLUDE_DIR' in os.environ and 'TOLUENE_OPENCL_LIBRARY_DIR' in os.environ:
            print('Found OpenCL at ' + os.environ['TOLUENE_OPENCL_INCLUDE_DIR'])
            opencl_include_dir = [os.environ['TOLUENE_OPENCL_INCLUDE_DIR']]
            opencl_library_dir = [os.environ['TOLUENE_OPENCL_LIBRARY_DIR']]
            found_opencl = True
    elif 'linux' in sys.platform:
        if 'TOLUENE_OPENCL_INCLUDE_DIR' in os.environ and 'TOLUENE_OPENCL_LIBRARY_DIR' in os.environ:
            print('Found OpenCL at ' + os.environ['TOLUENE_OPENCL_INCLUDE_DIR'])
            opencl_include_dir = [os.environ['TOLUENE_OPENCL_INCLUDE_DIR']]
            opencl_library_dir = [os.environ['TOLUENE_OPENCL_LIBRARY_DIR']]
            found_opencl = True
    elif 'win32' in sys.platform:
        if 'TOLUENE_OPENCL_INCLUDE_DIR' in os.environ and 'TOLUENE_OPENCL_LIBRARY_DIR' in os.environ:
            print('Found OpenCL at ' + os.environ['TOLUENE_OPENCL_INCLUDE_DIR'])
            opencl_include_dir = [os.environ['TOLUENE_OPENCL_INCLUDE_DIR']]
            opencl_library_dir = [os.environ['TOLUENE_OPENCL_LIBRARY_DIR']]
            found_opencl = True
        if 'CUDA_PATH' in os.environ:
            print('Found OpenCL at ' + os.environ['CUDA_PATH'])
            opencl_include_dir = [os.environ['CUDA_PATH'] + '/include']
            opencl_library_dir = [os.environ['CUDA_PATH'] + '/lib/x64']
            found_opencl = True

if not found_opencl:
    print('OpenCL not found. OpenCL is required for some extensions.')
    print('If you have OpenCL installed, please set the environment variable CUDA_PATH to the path of your OpenCL installation.')
    print('If you do not have OpenCL installed, you can install it from https://www.khronos.org/opencl/')
    print('If you do not want to install OpenCL, you can still use the rest of the library.')
else:
    extensions.append(
        Extension(
            'toluene_extensions.opencl.coordinates.transform',
            [
                'c/src/coordinates/state_vector.c',
                'c/src/math/constants.c',
                'c/src/math/linear_algebra.c',
                'c/src/models/earth/bias.c',
                'c/src/models/earth/constants.c',
                'c/src/models/earth/earth_orientation_parameters.c',
                'c/src/models/earth/nutation.c',
                'c/src/models/earth/polar_motion.c',
                'c/src/models/earth/precession.c',
                'c/src/models/earth/rotation.c',
                'c/src/models/moon/constants.c',
                'c/src/models/sun/constants.c',
                'c/src/opencl/coordinates/transform.c',
                'c/src/opencl/models/earth/nutation.c',
                'c/src/time/constants.c',
                'c/src/time/delta_t.c',
            ],
            include_dirs=['c/include'] + opencl_include_dir,
            library_dirs=opencl_library_dir,
            libraries=['OpenCL']
        ),
    )
    extensions.append(
        Extension(
            'toluene_extensions.opencl.context',
            [
                'c/src/opencl/context.c',
            ],
            include_dirs=['c/include'] + opencl_include_dir,
            library_dirs=opencl_library_dir,
            libraries=['OpenCL']
        )
    )


setup_args = dict(
    ext_modules=extensions,
)
setup(**setup_args)
