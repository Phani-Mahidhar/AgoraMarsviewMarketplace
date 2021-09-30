# MARSVIEW EXTENSION

**PREREQUISITES**: 

- **Android NDK**.
- **Socket.io** for c++ which can downloaded in their Github Page https://github.com/socketio/socket.io-client-cpp .
- OpenSSL is required for "https" connections using socket-io. These are usually built from the source for specific android hardware. But prebuilt binaries for Android were found at https://github.com/PurpleI2P/OpenSSL-for-Android-Prebuilt so instead they were used.
- [**AgoraLibs**](./src/main/agoraLibs) are the built files for agora extension development and [**AgoraRtcKit**](./src/main/cpp/AgoraRtcKit) contains the header files for the **AgoraLibs** source files. Both these are provided by agora and cant be downloaded elsewhere.



**BUILDING**:

- [**AgoraLibs**](./src/main/agoraLibs) folder contains the binaries for the agora extension. 

- [**AgoraRtcKit**](./src/main/cpp/AgoraRtcKit) contains the header files for the **AgoraLibs** source files.
  
- [**socket.io-client-cpp**](./src/main/cpp/socket.io-client-cpp) contains the socket-io source.

- [**plugin_soruce_code**](./src/main/cpp/plugin_source_code) contains the source and headers for the Marsview's extension.

[**build.gradle**](./build.gradle) in the extension module contains the build config for building the extension.

    ...
    android {
        ...

        defaultConfig {
            minSdkVersion 26
            targetSdkVersion 30

            testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
            consumerProguardFiles "consumer-rules.pro"

            externalNativeBuild {
                cmake {
                    cppFlags "-std=c++14"
                    abiFilters "armeabi-v7a", "arm64-v8a"
                    arguments "-DANDROID_STL=c++_shared"
                }
            }

            ...
        }
        ...

        externalNativeBuild {
            cmake {
                path "src/main/cpp/CMakeLists.txt"
                version "3.10.2"
            }
        }
        ndkVersion '21.0.6113669'
    }
    ...

[CMakeLists.txt](./src/main/cpp/CMakeLists.txt) file contains the build procedure for tying up all the binaries such as OpenSSL, Socket-io and the source code together and compile them.

**ENTRY POINT**: 
- [*agora-marsview.cpp*](src/main/cpp/agora-marsview.cpp) registers the Marsview's extension with the agora at the line. 


        // Register extension provider
        // No need to use quotation marks for input parameter of PROVIDER_NAME
        REGISTER_AGORA_EXTENSION_PROVIDER( Marsview, agora::extension::ExtensionProvider);

Once the Extension is registered the developer will be able to use the extension from his Android application. If the Extension is enabled this is the flow of audio in the extension module.

![flow](../docs/README%20images/../README%20images/audio_flow.png)
