cmake -H. -Bbuild\windows\Win64 -DSST_PROFILING=0 -DLST_LOGGING=1 -G"Visual Studio 15 2017 Win64" -DPYTHON_VERSION=3 -DPYWRAP_EXE="C:/Users/7oni/Laboratory/Compiler/llvm/build/bin/pywrap.exe"
cmake --build build\windows\Win64 --config Debug
