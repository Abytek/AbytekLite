# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("Application")

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

# Forces the linker to keep the JNI entry point for GameActivity
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

if(ANDROID_ABI STREQUAL "arm64-v8a")
    set(ABYTEK_EBT_PROCESSOR_ARCH "ARM_64")
elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
    set(ABYTEK_EBT_PROCESSOR_ARCH "ARM_32")
elseif(ANDROID_ABI STREQUAL "x86_64")
    set(ABYTEK_EBT_PROCESSOR_ARCH "X86_64")
elseif(ANDROID_ABI STREQUAL "x86")
    set(ABYTEK_EBT_PROCESSOR_ARCH "X86_32")
endif()
set(ABYTEK_EBT_CMAKE_SIGNATURE "${ABYTEK_EBT_ENVIRONMENT}-Android-Ninja-${CMAKE_BUILD_TYPE}-${ABYTEK_EBT_PROCESSOR_ARCH}")

set(CMakeBuildDirectorySignatureHashFile "${ABYTEK_PROJECT_DIR}/Build/CMakeBuildDirectorySignatureHash-${ABYTEK_EBT_ENVIRONMENT}-Android-Ninja-${CMAKE_BUILD_TYPE}-${ABYTEK_EBT_PROCESSOR_ARCH}.txt")
if(NOT EXISTS "${CMakeBuildDirectorySignatureHashFile}")
    message(FATAL_ERROR "Not found build directory signature file at: ${CMakeBuildDirectorySignatureHashFile}")
endif()
file(READ "${CMakeBuildDirectorySignatureHashFile}" CMakeBuildDirectorySignatureHash)

set(ABYTEK_ENGINE_BUILD_DIR "${ABYTEK_ENGINE_DIR}/Build/${CMakeBuildDirectorySignatureHash}")
set(ABYTEK_PROJECT_BUILD_DIR "${ABYTEK_PROJECT_DIR}/Build/${CMakeBuildDirectorySignatureHash}")
message(STATUS "Resolved ABYTEK_ENGINE_BUILD_DIR: ${ABYTEK_ENGINE_BUILD_DIR}")
message(STATUS "Resolved ABYTEK_PROJECT_BUILD_DIR: ${ABYTEK_PROJECT_BUILD_DIR}")

add_subdirectory("${ABYTEK_PROJECT_DIR}" "${ABYTEK_PROJECT_BUILD_DIR}")