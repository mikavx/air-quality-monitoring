# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "AirQualityMonitoring_autogen"
  "CMakeFiles\\AirQualityMonitoring_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AirQualityMonitoring_autogen.dir\\ParseCache.txt"
  )
endif()
