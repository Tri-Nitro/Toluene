from setuptools import setup, Extension

setup_args = dict(
    ext_modules=[
        Extension(
            'toluene_extensions.core_extensions',
            ['toluene/c_extensions/src/core.c',
             'toluene/c_extensions/src/earth_gravitational_model.c',
             'toluene/c_extensions/src/interpolation.c',
             'toluene/c_extensions/src/rotation_matrices.c',
             ],
            include_dirs=['toluene/c_extensions/include'],
            py_limited_api=True
        )
    ],
)
setup(**setup_args)
