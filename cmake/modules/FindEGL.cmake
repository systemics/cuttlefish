#
# Try to find EGL library and include path.
# Once done this will define
#
# EGL_FOUND
# EGL_INCLUDE_DIR
# EGL_LIBRARY
# 

message("finding EGL")
message("root folder set to: ${CMAKE_FIND_ROOT_FOLDER}")

if ((NOT EGL_INCLUDE_DIR) OR (NOT EGL_LIBRARY))
  FIND_PATH( EGL_INCLUDE_DIR EGL/egl.h
    /opt/vc/include
    DOC "The directory where EGL/egl.h resides")
  FIND_LIBRARY( EGL_LIBRARY
    NAMES EGL
    PATHS /opt/vc/lib 
    DOC "The directory where libEGL resides")
endif ((NOT EGL_INCLUDE_DIR) OR (NOT EGL_LIBRARY))
