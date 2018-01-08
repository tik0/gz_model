# 2.8.8 required to use PROTOBUF_IMPORT_DIRS
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
FIND_PACKAGE( Boost 1.40 COMPONENTS system filesystem program_options regex REQUIRED )
FIND_PACKAGE( Gazebo REQUIRED )

set (CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(SDF sdformat)
endif()

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${GAZEBO_PROTO_INCLUDE_DIRS}
  ${SDF_INCLUDE_DIRS}
  ${Boost_INCLUDES}
)

add_executable (gz gz.cc)
target_link_libraries(gz ${GAZEBO_LIBRARIES} ${GAZEBO_PROTO_LIBRARIES} ${SDF_LIBRARIES} ${Boost_LIBRARIES})
