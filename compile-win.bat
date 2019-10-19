cmake -H. -Bbuild\windows\Win64 -DSST_PROFILING=0 -DLST_LOGGING=1 -G"Visual Studio 15 2017 Win64"
cmake --build build\windows\Win64 --config Debug
