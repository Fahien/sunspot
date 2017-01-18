<img align="right" alt="SunSpoT" src="https://github.com/Fahien/sunspot/raw/master/sunspot.png" />
# SunSpoT
SunSpoT is a 3D engine capable of stereoscopic rendering, developed as a Computer Graphics project for my
Computer Science thesis at [ISISlab](http://www.isislab.it/), [Università degli Studi di Salerno](http://www.unisa.it/).

## Getting Started
SunSpoT is **open-source** and **cross-platform**, built and tested on Windows and Linux.

### Prerequisites
In order to build and run a SunSpoT application, you will need the following dependencies:

- [CMake](https://cmake.org/), Cross-Platform Make;
- [GLEW](http://glew.sourceforge.net/), The OpenGL Extension Wrangler Library;
- [GLFW](http://www.glfw.org/), OpenGL FrameWork;
- [SOIL](http://www.lonesock.net/soil.html), Simple OpenGL Image Library.

### Install
Create a project for your platform with cmake: `cmake -H. -Bbin`

If you would you can compile with cmake: `cmake --build bin --config Release`

Eventually, you can run the generated executable `bin\Release\sunspot.exe` on Windows or `bin/./sunspot` on Linux.

### Command line arguments
A SunSpoT executable accepts the following command line arguments:

- `-stereoscopic`: enable the stereoscopic rendering;
- `-scale <n>`: scale the window size by `n`;
- `-decorated`: add decorations to the window.

## Thanks
[All the ISISlab people](http://www.isislab.it/), in particular [Vittorio
Scarano](http://www.unisa.it/docenti/vittorioscarano/index?nv=docenti.unisa.it/vittorio.scarano), [Daniele
Monaco](#), and [Armando Faggiano](#).

A *special* thanks goes to [Angelo Theodorou](http://encelo.netsons.org/about/) for his guidance,
[Giovanni R. Ferrante](#) for the awesome SunSpoT logo, and [Maria Rosaria Minichiello](#) for her daily support.

## License
Apache License © 2017 [Antonio Caggiano](http://www.fahien.me/)
