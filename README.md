<img align="right" alt="SunSpoT"
src="https://bitbucket.org/Fahien/sunspot/avatar/256/?ts=1484573664" />
# SunSpoT
SunSpoT is a 3D engine capable of stereoscopic rendering, developed as a Computer Graphics project for my
Computer Science thesis at <a href="http://www.isislab.it/" target="_blank">ISISlab</a>, <a
href="http://www.unisa.it" target="_blank">Università degli Studi di Salerno</a>.

## Getting Started
SunSpoT is **open-source** and **cross-platform**, built and tested on Windows and Linux.

### Prerequisites
In order to build and run a SunSpoT application, you will need the following dependencies:

- <a href="https://cmake.org/" target="_blank">CMake</a>, Cross-Platform Make;
- <a href="http://glew.sourceforge.net/" target="_blank">GLEW</a>, The OpenGL Extension Wrangler Library;
- <a href="http://www.glfw.org/" target="_blank">GLFW</a>, OpenGL FrameWork;
- <a href="http://www.lonesock.net/soil.html" target="_blank">SOIL</a>, Simple OpenGL Image Library.

### Install
Create a project for your platform with cmake:

```
cmake -H. -Bbin
```

If you would you can compile with cmake:

```
cmake --build bin --config Release
```

Eventually, you can run the generated executable `bin\Release\sunspot.exe` on Windows or
`bin/./sunspot` on Linux.

## Thanks
<a href="http://www.isislab.it/" target="_blank">All ISISlab people</a>, in particular <a
href="http://www.unisa.it/docenti/vittorioscarano/index?nv=docenti.unisa.it/vittorio.scarano"
target="_blank">Vittorio Scarano</a>, <a href="#" target="_blank">Daniele Monaco</a>, and <a href="#"
target="_blank">Armando Faggiano</a>.

## License
Apache License © 2017 <a href="http://www.fahien.me/" target="_blank">Antonio Caggiano</a>
