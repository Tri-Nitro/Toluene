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
            'c/src/math/linear_algebra.c',
            'c/src/models/earth/polar_motion.c',
            'c/src/time/constants.c',
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
]

setup_args = dict(
    ext_modules=extensions,
)
setup(**setup_args)
