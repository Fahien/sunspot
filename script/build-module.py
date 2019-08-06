#!/usr/bin/python
"""Builds an extension module using distutils"""

import sys
import argparse
from distutils.core import setup, Extension

parser = argparse.ArgumentParser(description="Builds an extension module using distutils")

parser.add_argument('name', help="Name of the Python module")
parser.add_argument('sources', nargs='+', help="Sources of the module to compile")
parser.add_argument('--includes', nargs='+', help="Include directories" )
parser.add_argument('--libraries', nargs='+', help="Link libraries" )

args = parser.parse_args()

includes = []
if args.includes:
	includes = args.includes

libraries = []
if args.libraries:
	libraries = args.libraries

print("make module")
module = Extension( args.name, sources=args.sources, include_dirs=includes, extra_compile_args=['-std=c++17'], extra_objects=libraries)

print("setup")
sys.argv = sys.argv[:1]
sys.argv.append('build')
setup(name=args.name.capitalize(), version='1.0', description='Extension module', ext_modules=[module],)
print("done")
