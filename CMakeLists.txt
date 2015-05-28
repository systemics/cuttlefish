cmake_minimum_required(VERSION 2.8)
project(cuttlefish)

#from Toolchain file
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFINITIONS}")
#C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -ftemplate-depth-1200")
#TURN OFF A BUNCH OF WARNINGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-reorder -Wno-unused-function -Wno-sign-compare -Wno-unused-variable  -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#ADD SUBMODULES 
add_subdirectory(ext/vsr)
add_subdirectory(ext/gfx)

#GET INCLUDE DIRECTORIES, LINK DIRECTORIES, AND LINKED LIBS
get_property(GFX_LIBRARIES GLOBAL PROPERTY GFX_LIBRARIES)
get_property(GFX_INCLUDE_DIR GLOBAL PROPERTY GFX_INCLUDE_DIR)
get_property(GFX_LINK_DIR GLOBAL PROPERTY GFX_LINK_DIR)

get_property(VSR_LIBRARIES GLOBAL PROPERTY VSR_LIBRARIES)
get_property(VSR_INCLUDE_DIR GLOBAL PROPERTY VSR_INCLUDE_DIR)
get_property(VSR_LINK_DIR GLOBAL PROPERTY VSR_LINK_DIR)

file(GLOB graphicsGL ext/vsr/src/draw/GL/*.cpp)
file(GLOB graphicsGLES ext/vsr/src/draw/GLES/*.cpp)

#ADDITIONAL INCLUDES SPECIFIC TO CUTTLEFISH PROJECT
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ctl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/Gamma)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/cuttlebone)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/osc)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/Gamma/build/lib)

if(${RASPBERRY})

  message("****** COMPILING BINARIES FOR RASPBERRY PI ********")
set(libraries ${GFX_LIBRARIES} ${VSR_LIBRARIES})

include_directories(${GFX_INCLUDE_DIR} ${VSR_INCLUDE_DIR})
link_directories(${GFX_LINK_DIR} ${VSR_LINK_DIR})

   
  add_library(vsrgles ${graphicsGLES})

  find_package(Jack)
  find_package(RtAudio)

  set(libraries ${libraries} rt pthread vsrgles)
  set(libraries ${libraries} bcm_host vchiq_arm vcos ) #lo!! 

  #ADD ALL .CPP FILES IN EXAMPLES AND TESTS FOLDER AS TARGETS
  aux_source_directory(test source_list)
  aux_source_directory(example source_list)

  set(source_list ${source_list} example/swarm/pi.cpp) #INDIVIDUAL FILE
  set(source_list ${source_list} example/sync/pi.cpp) #INDIVIDUAL FILE

  #aux_source_directory(scratch source_list)
  foreach(source_file ${source_list})
    string(REGEX REPLACE "/" "_" target ${source_file})
    string(REGEX REPLACE ".cpp" "" target ${target})
    add_executable(${target} ${source_file})
    target_link_libraries(${target} ${libraries})
  endforeach(source_file)

elseif(${APPLE})
   
  message("****** COMPILING BINARIES FOR OSX ********")
set(libraries ${GFX_LIBRARIES} ${VSR_LIBRARIES})

include_directories(${GFX_INCLUDE_DIR} ${VSR_INCLUDE_DIR})
link_directories(${GFX_LINK_DIR} ${VSR_LINK_DIR})


  add_library(vsrgl ${graphicsGL} ${graphicsGLES}) 
 
  set(libraries ${libraries} vsrgl)

  #ADD ALL .CPP FILES IN EXAMPLES AND TESTS FOLDER AS TARGETS
  aux_source_directory(example/control source_list)

  set(source_list ${source_list} example/sync/sim.cpp) #INDIVIDUAL FILE
  set(source_list ${source_list} example/swarm/desktop_sim.cpp) #INDIVIDUAL FILE

  foreach(source_file ${source_list})
    string(REGEX REPLACE "/" "_" target ${source_file})
    string(REGEX REPLACE ".cpp" "" target ${target})
    add_executable(${target} ${source_file})
    target_link_libraries(${target} ${libraries})
  endforeach(source_file)

elseif(${JETSON})

  message("****** COMPILING BINARIES FOR JETSON ********")
set(libraries ${VSR_LIBRARIES})

include_directories(${VSR_INCLUDE_DIR})
link_directories(${VSR_LINK_DIR})


  #add_library(vsrgl ${graphicsGL} ${graphicsGLES}) 
 
  #set(libraries ${libraries} vsrgl)

  #ADD ALL .CPP FILES IN EXAMPLES AND TESTS FOLDER AS TARGETS
  ####aux_source_directory(example/control source_list)

  #####set(source_list ${source_list} example/swarm/sim.cpp) #INDIVIDUAL FILE
  set(source_list ${source_list} example/sync/sim.cpp) #INDIVIDUAL FILE

  foreach(source_file ${source_list})
    string(REGEX REPLACE "/" "_" target ${source_file})
    string(REGEX REPLACE ".cpp" "" target ${target})
    add_executable(${target} ${source_file})
    target_link_libraries(${target} ${libraries})
  endforeach(source_file)
  
endif()


#THESE ARE INCLUDED BY GFX ALREADY:
#include_directories(${CMAKE_FIND_ROOT_PATH}/usr/include)
#include_directories(${CMAKE_FIND_ROOT_PATH}/usr/local/include)
#include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include)
#include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos/pthreads)
#include_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vmcs_host/linux)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/vsr/build/lib)

#link_directories(${CMAKE_FIND_ROOT_PATH}/usr/lib)
#link_directories(${CMAKE_FIND_ROOT_PATH}/usr/local/lib)
#link_directories(${CMAKE_FIND_ROOT_PATH}/opt/vc/lib)




#PRINT OUT DIRECTORIES
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
#foreach(d ${dirs})
#message(STATUS "link dir='${d}'")
#endforeach()
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(d ${dirs})
#message(STATUS "include dir='${d}'")
#endforeach()



#Is Crypto and Zlib Necessary?
#find_package(Crypto REQUIRED)
#set(libraries ${libraries} ${CRYPTO_LIBRARIES})
#find_package(ZLIB REQUIRED)
#set(libraries ${libraries} ${ZLIB_LIBRARIES})
#include_directories(${ZLIB_INCLUDE_DIRS} )
#include_directories(${CRYPTO_INCLUDE_DIR})

#LDFLAGS += -lbcm_host
#LDFLAGS += -llo
#LDFLAGS += -lm
#LDFLAGS += -lrt
#LDFLAGS += -lstdc++
#LDFLAGS += -lvchiq_arm
#LDFLAGS += -lvcos
#LDFLAGS += -lvsr

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
#uendif
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
