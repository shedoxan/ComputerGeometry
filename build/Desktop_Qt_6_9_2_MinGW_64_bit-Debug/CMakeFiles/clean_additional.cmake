# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "src\\apps\\Viewer2d\\CMakeFiles\\Viewer2d_autogen.dir\\AutogenUsed.txt"
  "src\\apps\\Viewer2d\\CMakeFiles\\Viewer2d_autogen.dir\\ParseCache.txt"
  "src\\apps\\Viewer2d\\Viewer2d_autogen"
  "src\\lib\\PlaneGeometry\\CMakeFiles\\PlaneGeometry_autogen.dir\\AutogenUsed.txt"
  "src\\lib\\PlaneGeometry\\CMakeFiles\\PlaneGeometry_autogen.dir\\ParseCache.txt"
  "src\\lib\\PlaneGeometry\\PlaneGeometry_autogen"
  "src\\tests\\CMakeFiles\\PlaneGeometryTests_autogen.dir\\AutogenUsed.txt"
  "src\\tests\\CMakeFiles\\PlaneGeometryTests_autogen.dir\\ParseCache.txt"
  "src\\tests\\PlaneGeometryTests_autogen"
  )
endif()
