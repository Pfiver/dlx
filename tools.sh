push() { ~/Volatile/Workbench/adt-bundle-mac-x86_64-20131030/sdk/platform-tools/adb push libs/armeabi/suds /data/.bin; }
run() { ~/Volatile/Workbench/adt-bundle-mac-x86_64-20131030/sdk/platform-tools/adb shell "/data/.bin/suds $* < /sdcard/2015-10-11.sud"; }
build() { ~/Volatile/Workbench/android-ndk-r10e-darwin-x86_64/ndk-build; }
