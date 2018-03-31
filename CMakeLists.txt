cmake_minimum_required (VERSION 2.8)

include_directories("${CMAKE_SOURCE_DIR}/tycho/third_party/freetype/include")

tycho_add_library(graphics "tyfreetype;tycore;tyimage;${link_libs}" "libs")



