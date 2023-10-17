cmake_minimum_required(VERSION 3.0)
project (metalluga VERSION 0.0.3 LANGUAGES CXX C)


set(LV2_INSTALL_DIR lib/lv2 CACHE PATH "Specifies where the LV2 libraries should be installed")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O2 ")


option(USE_CLANG "Build with Clang" OFF)


add_definitions ( -DPIC )

if(USE_CLANG)
  set(CMAKE_CXX_COMPILER clang++)
  set(CMAKE_C_COMPILER clang)
  message("+ Build with Clang++")
endif(USE_CLANG)


find_package(PkgConfig)
pkg_check_modules(LV2 REQUIRED lv2>=0.1.0)

include_directories (${LV2_INCLUDE_DIRS})
link_directories (${LV2_LIBRARY_DIRS} )



add_library(metalluga SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/fx-resofilter.h
  ./source/fx-resofilter.cpp
  ./source/metalluga.cpp
)


add_library(charm SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/charm.cpp
)


set_target_properties(metalluga  PROPERTIES PREFIX "")
target_link_libraries(metalluga  ${LV2_LIBRARIES})

set_target_properties(charm  PROPERTIES PREFIX "")
target_link_libraries(charm  ${LV2_LIBRARIES})


# Metalluga install
install(TARGETS metalluga
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/metalluga.lv2
)

install (FILES ./source/ttl-metalluga/metalluga.ttl ./source/ttl-metalluga/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/metalluga.lv2
)

# Charm install
install(TARGETS charm
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/charm.lv2
)

install (FILES ./source/ttl-charm/charm.ttl ./source/ttl-charm/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/charm.lv2
)

