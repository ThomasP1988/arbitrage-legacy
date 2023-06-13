# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and 
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 3.8.2)

# Set project name, version and laguages here. (change as needed)
# Version numbers are available by including "exampleConfig.h" in 
# the source. See exampleConfig.h.in for some more details.
project(Ctrade VERSION 0.0.0.1 LANGUAGES CXX)

set(WITH_EXPERIMENTAL_FEATURES OFF)

# Options: Things you can set via commandline options to cmake (e.g. -DENABLE_LTO=[ON|OFF])
option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." ON)

# Include stuff. No change needed.
set(CMAKE_CXX_FLAGS "-pthread")
set(CURL_LIBRARY "-lcurl")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(Boost_FOUND)
find_package(Boost 1.74.0 EXACT)
include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(external/Disruptor)
add_subdirectory(external/tgbot-cpp-master)
# add_subdirectory(external/tgbot-cpp-master/include/tgbot)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

link_libraries(OpenSSL::SSL)
link_libraries(OpenSSL::Crypto)

# include(external/oneTBB/cmake/TBBGet.cmake)
# include(external/oneTBB/cmake/TBBBuild.cmake)
# tbb_get(TBB_ROOT tbb_root SOURCE_CODE)
# tbb_build(TBB_ROOT ${tbb_root} CONFIG_DIR TBB_DIR)
# find_package(TBB REQUIRED)

# include(external/oneTBB-onetbb_2021/TBBConfig.cmake)
# include(external/oneTBB-onetbb_2021/TBBConfigVersion.cmake)

# find_package(TBB REQUIRED)

include( FetchContent )
FetchContent_Declare(
        daw_json_link
        GIT_REPOSITORY https://github.com/beached/daw_json_link
        GIT_TAG  v2.4.0
        CMAKE_ARGS "-Wno-everything"
)
FetchContent_MakeAvailable(daw_json_link)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           master
    SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
    BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
)
FetchContent_MakeAvailable(gtest)

FetchContent_Declare(
        TBB
        GIT_REPOSITORY https://github.com/oneapi-src/oneTBB
        GIT_TAG master
        # CMAKE_ARGS "-Wno-everything"
)
FetchContent_MakeAvailable(TBB)


include_directories(
    ${CURL_INCLUDE_DIR}
)

include(ConfigSafeGuards)
include(Colors)
include(CTest)
include(Doctest)
include(Documentation)
include(LTO)
include(Misc)
include(RapidJson)
include(Warnings)
include(Binance)



# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
#                         Locate files (change as needed).
# --------------------------------------------------------------------------------
set(SOURCES          # All .cpp files in src/
    src/platforms/binance/platform.cc
    src/shared/markets/state.cc
    src/shared/markets/path.cc
    src/strategies/simple_strategie.cc
    # src/shared/thread/compare_price.cc
)

set(TESTFILES        # All .cpp files in tests/
    tests/main.cpp
)
set(LIBRARY_NAME ctrade)  # Default name for the library built from src/*.cpp (change if you wish)


# --------------------------------------------------------------------------------
#                            Build! (Change as needed)
# --------------------------------------------------------------------------------
# Compile all sources into a library.
add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

# Lib needs its header files, and users of the library must also see these (PUBLIC). (No change needed)
target_include_directories(${LIBRARY_NAME} PUBLIC 
${PROJECT_SOURCE_DIR}/include 
${PROJECT_SOURCE_DIR}/external/Disruptor
${PROJECT_SOURCE_DIR}/external/oneTBB/include
)

# There's also (probably) doctests within the library, so we need to see this as well.
target_link_libraries(${LIBRARY_NAME} PUBLIC TgBot gtest gmock binacpp daw::json_link DisruptorStatic boost_thread TBB::tbb)

# Set the compile options you want (change as needed).
target_set_warnings(${LIBRARY_NAME} ENABLE ALL AS_ERROR ALL DISABLE Annoying reorder)
# target_compile_options(${LIBRARY_NAME} ... )  # For setting manually.

# Add an executable for the file app/main.cpp.
# If you add more executables, copy these lines accordingly.
add_executable(main app/main.cpp)   # Name of exec. and location of file.
target_link_libraries(main PRIVATE ${LIBRARY_NAME})  # Link the executable to library (if it uses it).
target_set_warnings(main ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Set warnings (if needed).
target_enable_lto(main optimized)  # enable link-time-optimization if available for non-debug configurations

# Set the properties you require, e.g. what C++ standard to use. Here applied to library and main (change as needed).
set_target_properties(
    ${LIBRARY_NAME} main 
      PROPERTIES 
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED NO 
        CXX_EXTENSIONS ON
)

# Set up tests (see tests/CMakeLists.txt).
add_subdirectory(tests)
