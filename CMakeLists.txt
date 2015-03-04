cmake_minimum_required (VERSION 2.8)

include_directories("${CMAKE_SOURCE_DIR}/lib/3rdparty/freetype/include")

tycho_add_library(graphics "tyfreetype;tycore;tyimage;${link_libs}" "libs")



