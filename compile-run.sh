cmake -H. -Bbin -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
make -C bin -j4
bin/./sunspot
