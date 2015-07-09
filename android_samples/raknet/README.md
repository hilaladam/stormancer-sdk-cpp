#Raknet for android
The compiled lib is available in the libs/armeabi-v7a subdirectory.

## How to build
If you  want to rebuild raknet.

### Links
- Source : https://github.com/OculusVR/RakNet

Read the **Preparation** part here before continuing:  
https://github.com/Stormancer/stormancer-sdk-cpp/blob/master/android_samples/stormancer/README.md

###On Windows
Run a windows cmd and type:  
```
cd c:\cygwin64\home\username\android-ndk-r10\samples\raknet\
mklink /D src C:\Users\username\stormancer-sdk-cpp\RakNet\Source
```

Run cygwin64 and type:  
```
cd android-ndk-r10/samples/raknet
../../ndk-build
```