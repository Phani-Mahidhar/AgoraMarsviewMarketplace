# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

project(agora-marsview)

#link agora so
set(agora-lib-so ${PROJECT_SOURCE_DIR}/../agoraLibs/${CMAKE_ANDROID_ARCH_ABI}/libagora-rtc-sdk-jni.so)
link_libraries(${agora-lib-so})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(openssl_root_dir ${PROJECT_SOURCE_DIR}/../openssl)
set(openssl_libraries_dir ${openssl_root_dir}/${CMAKE_ANDROID_ARCH_ABI}/lib)

set(OPENSSL_ROOT_DIR ${openssl_root_dir}/${CMAKE_ANDROID_ARCH_ABI})
set(OPENSSL_INCLUDE_DIR ${openssl_root_dir}/include)
set(OPENSSL_CRYPTO_LIBRARY ${openssl_root_dir}/${CMAKE_ANDROID_ARCH_ABI}/lib/libcrypto.a)
set(OPENSSL_SSL_LIBRARY ${openssl_root_dir}/${CMAKE_ANDROID_ARCH_ABI}/lib/libssl.a)
set(OPENSSL_LIBRARIES "ssl" "crypto")

#link_directories(${openssl_libraries_dir})
include_directories(${OPENSSL_INCLUDE_DIR})

set(socket-io_root ./socket.io-client-cpp)
set(socket-io_build-dir ${socket-io_root}/builds/android)

add_subdirectory( ${socket-io_root} ${socket-io_build-dir})

#find_package(OpenSSL REQUIRED)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        agora-marsview

             # Sets the library as a shared library.
        SHARED
        plugin_source_code/ExtensionProvider.cpp
        plugin_source_code/ExtensionAudioFilter.cpp
        plugin_source_code/JniHelper.cpp
        plugin_source_code/AudioTranscriptProvider.cpp
        plugin_source_code/AudioStreamHelper.cpp
        # Provides a relative path to your source file(s).
        agora-marsview.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
#target_include_directories(agora-marsview PRIVATE ./plugin_source_code)

target_include_directories(agora-marsview PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries( # Specifies the target library.
                        agora-marsview
                        ${agora-lib-so}

                        ${OPENSSL_INCLUDE_DIR}
                        ${OPENSSL_SSL_LIBRARY}
                        ${OPENSSL_CRYPTO_LIBRARY}

                        sioclient_tls
                        sioclient
                       # Links the target library to the log library
                       # included in the NDK.
                        ${log-lib}
                        android)
