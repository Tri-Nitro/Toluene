from setuptools import setup, Extension

extensions = [
    Extension(
        'toluene.extensions.math.algebra',
        ['c/src/math/algebra.c'],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene.extensions.models.earth.ellipsoid',
        ['c/src/models/earth/ellipsoid.c'],
        include_dirs=['c/include'],
    ),
    Extension(
        'toluene.extensions.models.earth.nutation',
        ['c/src/models/earth/nutation.c'],
        include_dirs=['c/include'],
    ),
]

setup_args = dict(
    ext_modules=extensions,
)
setup(**setup_args)
