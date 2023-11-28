from setuptools import setup, Extension

setup_args = dict(
    ext_modules=[
        Extension(
            'toluene_extensions.util.config',
            [
                'toluene/c_extensions/src/util/config.c'
            ],
            include_dirs=['toluene/c_extensions/include'],
            py_limited_api=True
        ),
        Extension(
            'toluene_extensions.models.earth.model',
            [
                'toluene/c_extensions/src/models/earth/model.c'
            ],
            include_dirs=['toluene/c_extensions/include'],
            py_limited_api=True
        ),
        Extension(
            'toluene_extensions.core_extensions',
            [
             'toluene/c_extensions/src/earth_gravitational_model.c',
             'toluene/c_extensions/src/egm84.c',
             'toluene/c_extensions/src/interpolation.c',
             'toluene/c_extensions/src/matrix.c',
             'toluene/c_extensions/src/polynomial.c',
             'toluene/c_extensions/src/rotation_matrices.c',
             'toluene/c_extensions/src/core.c',
             ],
            include_dirs=['toluene/c_extensions/include'],
            py_limited_api=True
        )
    ],
)
setup(**setup_args)
