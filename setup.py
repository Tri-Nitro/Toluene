from setuptools import setup, Extension

setup_args = dict(
    ext_modules=[
        Extension(
            'toluene_extensions.base_extensions',
            ['toluene/c_extensions/base.c'],
            include_dirs=['toluene/c_extensions'],
            py_limited_api=True
        )
    ]
)
setup(**setup_args)