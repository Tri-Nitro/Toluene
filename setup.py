from setuptools import setup, Extension

setup_args = dict(
    ext_modules=[
        Extension(
            'toluene_extensions.core_extensions',
            ['toluene/c_extensions/src/core.c'],
            include_dirs=['toluene/c_extensions/include'],
            py_limited_api=True
        )
    ],
)
setup(**setup_args)