cmake_minimum_required(VERSION 3.0)
project (bedroomstudio VERSION 1.0.0 LANGUAGES CXX C)


set(LV2_INSTALL_DIR lib/lv2 CACHE PATH "Specifies where the LV2 libraries should be installed")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O2 ")


option(USE_CLANG "Build with Clang" OFF)


#add_definitions ( -DPIC )

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

add_library(grelka SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/fx-resofilter.h
  ./source/fx-resofilter.cpp
  ./source/grelka.cpp
)


add_library(bronza SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/fx-resofilter.h
  ./source/fx-resofilter.cpp
  ./source/bronza.cpp
)


add_library(charm SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/charm.cpp
)


add_library(flanger SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/flanger.cpp
)

add_library(eq SHARED
  ./source/dsp.cpp
  ./source/dsp.h
  ./source/eq.cpp
)


set_target_properties(metalluga  PROPERTIES PREFIX "")
target_link_libraries(metalluga  ${LV2_LIBRARIES})

set_target_properties(grelka  PROPERTIES PREFIX "")
target_link_libraries(grelka  ${LV2_LIBRARIES})

set_target_properties(charm  PROPERTIES PREFIX "")
target_link_libraries(charm  ${LV2_LIBRARIES})

set_target_properties(bronza PROPERTIES PREFIX "")
target_link_libraries(bronza ${LV2_LIBRARIES})

set_target_properties(flanger PROPERTIES PREFIX "")
target_link_libraries(flanger ${LV2_LIBRARIES})

set_target_properties(eq PROPERTIES PREFIX "")
target_link_libraries(eq ${LV2_LIBRARIES})



# Metalluga install
install(TARGETS metalluga
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/metalluga.lv2
)

install (FILES ./source/ttl-metalluga/metalluga.ttl ./source/ttl-metalluga/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/metalluga.lv2
)


# Grelka install
install(TARGETS grelka
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/grelka.lv2
)

install (FILES ./source/ttl-grelka/grelka.ttl ./source/ttl-grelka/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/grelka.lv2
)


# Metalluga install
install(TARGETS bronza
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/bronza.lv2
)

install (FILES ./source/ttl-bronza/bronza.ttl ./source/ttl-bronza/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/bronza.lv2
)


# Charm install
install(TARGETS charm
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/charm.lv2
)

install (FILES ./source/ttl-charm/charm.ttl ./source/ttl-charm/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/charm.lv2
)



# Charm install
install(TARGETS flanger
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/flanger.lv2
)

install (FILES ./source/ttl-flanger/flanger.ttl ./source/ttl-flanger/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/flanger.lv2
)



# Charm install
install(TARGETS eq
  LIBRARY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/eq.lv2
)

install (FILES ./source/ttl-eq/eq.ttl ./source/ttl-eq/manifest.ttl
  DESTINATION ${CMAKE_INSTALL_PREFIX}/${LV2_INSTALL_DIR}/eq.lv2
)

