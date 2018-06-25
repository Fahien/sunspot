CMAKE="/home/fahien/tools/cmake"
OUTPUT=deploy
NDK=/opt/android-ndk
STL=c++_shared

$CMAKE -version

# Android armeabi-v7a
$CMAKE -H. -Bbuild/android/armeabi-v7a -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_NDK=$NDK -DCMAKE_ANDROID_STL_TYPE=$STL -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang -DCMAKE_INSTALL_PREFIX=$OUTPUT
$CMAKE --build build/android/armeabi-v7a
