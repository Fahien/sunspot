cmake -H. -Bbuild\windows -DSST_PROFILING=0 -DLST_LOGGING=1 -G"Visual Studio 15 2017 Win64" -T"llvm-vs2014"
cmake --build build\windows --config Release
