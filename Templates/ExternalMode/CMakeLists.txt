cmake_minimum_required(VERSION 3.14)


project(ExternalMode LANGUAGES C CXX)

set(ABYTEK_EBT_EXTERNAL_MODE ON)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../.." "${CMAKE_CURRENT_LIST_DIR}/Build/Abytek")