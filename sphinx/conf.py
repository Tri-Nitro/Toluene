# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html
import os
import sys

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'toluene'
copyright = '2023, Tri-nitro'
author = 'Tri-nitro'
release = '0.0.23'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

sys.path.insert(0, os.path.abspath('../'))
sys.path.insert(0, os.path.abspath('../toluene'))
sys.path.insert(0, os.path.abspath('../toluene/base'))
sys.path.insert(0, os.path.abspath('../toluene/elevation'))
sys.path.insert(0, os.path.abspath('../toluene/image'))
sys.path.insert(0, os.path.abspath('../toluene/test'))
sys.path.insert(0, os.path.abspath('../toluene/util'))
extensions = ['sphinx.ext.autodoc']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
