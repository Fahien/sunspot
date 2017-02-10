cmake -H. -Bbin -DSST_PROFILING=1
cmake --build bin --config Release
bin\Release\sunspot.exe -stereoscopic -scale 4 > output.dat
pause
