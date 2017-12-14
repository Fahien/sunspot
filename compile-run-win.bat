cmake -H\. -Bbuild -DSST_PROFILING=0 -DSST_LOGGING=1
cmake --build build --config Release
build\Release\sunspot.exe
