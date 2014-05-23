#  ALSA_FOUND - system has Sound
#  ALSA_INCLUDE_DIRS - the Sound include directory
#  ALSA_LIBRARIES - Link these to use Sound

if (ALSA_LIBRARIES AND ALSA_INCLUDE_DIRS)
  # in cache already
  set(ALSA_FOUND TRUE)
else (ALSA_LIBRARIES AND ALSA_INCLUDE_DIRS)

  find_path(ALSA_INCLUDE_DIR
    NAMES
      alsa/asoundlib.h
    PATHS
      /usr/include
  )

  find_library(ALSA_LIBRARY
    NAMES
      asound
    PATHS
      /usr/local/lib
      /usr/lib/arm-linux-gnueabihf
  )

  set(ALSA_INCLUDE_DIRS ${ALSA_INCLUDE_DIR})
  set(ALSA_LIBRARIES ${ALSA_LIBRARY})

  if (ALSA_INCLUDE_DIRS AND ALSA_LIBRARIES)
    set(ALSA_FOUND TRUE)
  endif (ALSA_INCLUDE_DIRS AND ALSA_LIBRARIES)

  if (ALSA_FOUND)
	  message(STATUS "Found Sound: ${ALSA_LIBRARIES}")
  else (ALSA_FOUND)
    if (Sound_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Sound")
    endif (Sound_FIND_REQUIRED)
  endif (ALSA_FOUND)

  # show the ALSA_INCLUDE_DIRS and ALSA_LIBRARIES variables only in the advanced view
  # mark_as_advanced(ALSA_INCLUDE_DIRS ALSA_LIBRARIES)

endif (ALSA_LIBRARIES AND ALSA_INCLUDE_DIRS)
