CMAKE="/home/fahien/tools/cmake"
OUTPUT=deploy

$CMAKE -version

# Linux x86_64
$CMAKE -H. -Bbuild/linux/x86_64 -DLST_LOGGING=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$OUTPUT
$CMAKE --build build/linux/x86_64