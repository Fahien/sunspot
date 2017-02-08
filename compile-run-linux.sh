cmake -H. -Bbin -DSST_PROFILING=1 -DSST_LOGGING=0
make -C bin -j4
bin/./sunspot -stereoscopic -scale 4
