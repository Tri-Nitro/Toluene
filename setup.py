from setuptools import setup, Extension

extensions = [
    Extension(
        'toluene.extensions.models.earth.ellipsoid',
        [
            'c/src/models/earth/ellipsoid.c',
        ],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene.extensions.models.earth.nutation',
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
