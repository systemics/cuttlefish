cmake_minimum_required(VERSION 2.8)
project(cuttlefish)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

#find_package(Crypto REQUIRED)
#set(libraries ${libraries} ${CRYPTO_LIBRARIES})
#find_package(ZLIB REQUIRED)
#set(libraries ${libraries} ${ZLIB_LIBRARIES})
#include_directories(${ZLIB_INCLUDE_DIRS} )
#include_directories(${CRYPTO_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/Gamma)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/cuttlebone)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/gfx)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/gfx/gfx)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/osc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/vsr)

include_directories(${CMAKE_FIND_ROOT_PATH}/usr/include)
include_directories(${CMAKE_FIND_ROOT_PATH}/usr/local/include)
include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include)
include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos/pthreads)
include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vmcs_host/linux)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ms_demos)

if(UNIX AND NOT APPLE)
  set(libraries ${libraries} rt pthread)
endif()

aux_source_directory(test source_list)
aux_source_directory(example source_list)
#aux_source_directory(scratch source_list)
foreach(source_file ${source_list})
  string(REGEX REPLACE "/" "_" target ${source_file})
  string(REGEX REPLACE ".cpp" "" target ${target})
  add_executable(${target} ${source_file})
  target_link_libraries(${target} ${libraries})
endforeach(source_file)

#DIRECTORIES
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(d ${dirs})
message(STATUS "link dir='${d}'")
endforeach()
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(d ${dirs})
message(STATUS "include dir='${d}'")
endforeach()


#CXXFLAGS = -D __raspberry__ -D __LINUX_ALSA__

#IPATH += -Ictl
#IPATH += -I$(PIROOT)usr/include
#IPATH += -I$(PIROOT)usr/local/include
#IPATH += -I$(PIROOT)opt/vc/include
#IPATH += -I$(PIROOT)opt/vc/include/interface/vcos/pthreads
#IPATH += -I$(PIROOT)opt/vc/include/interface/vmcs_host/linux
#IPATH += -I$(EXT_DIR)/vsr/
#IPATH += -I$(EXT_DIR)/gfx/
#IPATH += -I$(EXT_DIR)/gfx/gfx/
#IPATH += -I$(EXT_DIR)/cuttlebone/
#IPATH += -I$(EXT_DIR)/Gamma/
#
#LDFLAGS += -L$(PIROOT)lib
#LDFLAGS += -L$(PIROOT)usr/lib
#LDFLAGS += -L$(PIROOT)usr/local/lib
#LDFLAGS += -L$(PIROOT)opt/vc/lib 
#LDFLAGS += -L$(EXT_DIR)Gamma/build/lib
#LDFLAGS += -L$(EXT_DIR)vsr/build/lib
##LDFLAGS += -L$(PIROOT)lib/arm-linux-gnueabihf
##LDFLAGS += -L$(PIROOT)usr/lib/arm-linux-gnueabihf
#LDFLAGS += -lEGL
#LDFLAGS += -lGLESv2
##LDFLAGS += -lGamma
#LDFLAGS += -lpthread
##LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libportaudio.a
##LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libjack.a
#
#OBJ_FILES = 
#
#ifeq (1,$(AUDIOBUILD))
#LDFLAGS += -lGamma
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libasound.so
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libsndfile.a
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libFLAC.so
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libvorbis.so
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libvorbisenc.so
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libogg.so
#OBJ_FILES += src/ctl_sound.cpp src/RtAudio.cpp
##LDFLAGS += -lsndfile1
##LDFLAGS += -lasound
##LDFLAGS += -ljack
##LDFLAGS += -lportaudio
#endif
#
#
#
#LDFLAGS += -lbcm_host
#LDFLAGS += -llo
#LDFLAGS += -lm
#LDFLAGS += -lrt
#LDFLAGS += -lstdc++
#LDFLAGS += -lvchiq_arm
#LDFLAGS += -lvcos
#LDFLAGS += -lvsr
#
#
