#  SOUND_FOUND - system has Sound
#  SOUND_INCLUDE_DIRS - the Sound include directory
#  SOUND_LIBRARIES - Link these to use Sound

if (SOUND_LIBRARIES AND SOUND_INCLUDE_DIRS)
  # in cache already
  set(SOUND_FOUND TRUE)
else (SOUND_LIBRARIES AND SOUND_INCLUDE_DIRS)

  find_path(SOUND_INCLUDE_DIR
    NAMES
      Sound.h
    PATHS
      ${CMAKE_SOURCE_DIR}/..
      ${CMAKE_CURRENT_SOURCE_DIR}/..
  )

  find_library(SOUND_LIBRARY
    NAMES
      Sound
      libSound.a
    PATHS
      ${CMAKE_SOURCE_DIR}/..
      ${CMAKE_CURRENT_SOURCE_DIR}/..
  )

  if (${SOUND_LIBRARY} STREQUAL "")
    find_path(SOUND_LIBRARY
      libSound.a
    PATHS
      ${CMAKE_SOURCE_DIR}/..
      ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
  endif()


  set(SOUND_INCLUDE_DIRS ${SOUND_INCLUDE_DIR})
  set(SOUND_LIBRARIES ${SOUND_LIBRARY})

  if (SOUND_INCLUDE_DIRS AND SOUND_LIBRARIES)
    set(SOUND_FOUND TRUE)
  endif (SOUND_INCLUDE_DIRS AND SOUND_LIBRARIES)

  if (SOUND_FOUND)
	  message(STATUS "Found Sound: ${SOUND_LIBRARIES}")
  else (SOUND_FOUND)
    if (Sound_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Sound")
    endif (Sound_FIND_REQUIRED)
  endif (SOUND_FOUND)

  # show the SOUND_INCLUDE_DIRS and SOUND_LIBRARIES variables only in the advanced view
  # mark_as_advanced(SOUND_INCLUDE_DIRS SOUND_LIBRARIES)

endif (SOUND_LIBRARIES AND SOUND_INCLUDE_DIRS)
