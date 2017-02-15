from distutils.core import setup, Extension
import numpy

mod = Extension('Normalize',
    include_dirs = [numpy.get_include()],
    sources = ['Normalize.c'],
    extra_compile_args=['-O2', '-fopenmp'],
    extra_link_args=['-lgomp', '-lit']
)

setup (name = 'Normalize',
    author = 'Aljoscha Rheinwalt',
    author_email = 'aljoscha.rheinwalt@uni-potsdam.de',
    ext_modules = [mod]
)
