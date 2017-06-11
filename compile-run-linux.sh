cmake -H. -Bbin -DSST_PROFILING=0 -DSST_LOGGING=1
make -C bin -j4
bin/./sunspot -stereoscopic -scale 4
