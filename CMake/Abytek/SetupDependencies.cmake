
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/GitHelper)

include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)


#####################################################################################
#   Setup directory for dependencies to be downloaded into
#####################################################################################
if(NOT EXISTS ${ABYTEK_DEPENDENCIES_DIR})
    file(MAKE_DIRECTORY ${ABYTEK_DEPENDENCIES_DIR})
endif()


Abytek_SetGlobal(BUILD_SHARED_LIBS OFF)


# Download Boost
message(STATUS "Fetching Boost...")
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
    boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.zip
    USES_TERMINAL_DOWNLOAD TRUE
    DOWNLOAD_NO_EXTRACT FALSE
)
FetchContent_MakeAvailable(boost)
if(boost_POPULATED)
    message(STATUS "Boost fetched and available.")
else()
    message(FATAL_ERROR "Failed to fetch Boost.")
endif()


# Download CPPDebuggerVisualizers
message(STATUS "Fetching CPPDebuggerVisualizers...")
if (MSVC)
    FetchContent_Declare(
        cpp_debugger_visualizers
        GIT_REPOSITORY https://github.com/KindDragon/CPPDebuggerVisualizers.git
        GIT_TAG 11e0f2277d151906093dfd018dcae7dd6173fe65
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(cpp_debugger_visualizers)
    if(cpp_debugger_visualizers_POPULATED)
        message(STATUS "CPPDebuggerVisualizers fetched and available.")
    else()
        message(FATAL_ERROR "Failed to fetch CPPDebuggerVisualizers.")
    endif()
endif()


# Find doxygen
find_package(Doxygen)


# Download doxygen-awesome-css
message(STATUS "Fetching doxygen-awesome-css...")
FetchContent_Declare(
    doxygen_awesome_css
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG 8cea9a073ecd50a5b2c0958a3df100292d6c7374
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(doxygen_awesome_css)
if(doxygen_awesome_css_POPULATED)
    message(STATUS "doxygen-awesome-css fetched and available.")
else()
    message(FATAL_ERROR "Failed to fetch doxygen-awesome-css.")
endif()


# Find NodeJS
find_program(
    ABYTEK_NODEJS_EXECUTABLE 
    NAMES node nodejs
    HINTS
    $ENV{NODE_DIR}
    PATH_SUFFIXES bin
    DOC "Node.js executable for CMakeJS"
    REQUIRED
)
Abytek_SetGlobal(ABYTEK_NODEJS_EXECUTABLE "${ABYTEK_NODEJS_EXECUTABLE}")