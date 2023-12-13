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
            'c/src/coordinates/transform.c'
        ],
        include_dirs=['c/include']
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
        ],
        include_dirs=['c/include'],
    ),
]

setup_args = dict(
    ext_modules=extensions,
)
setup(**setup_args)
