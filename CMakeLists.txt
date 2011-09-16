CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# set Debug builds as default (if not already changed in the cache)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF()

FIND_PACKAGE( GTK2 )
INCLUDE_DIRECTORIES( ${GTK2_INCLUDE_DIRS} inc )

ADD_DEFINITIONS(
    -Wextra
    -Werror
    -Wunused
#   -Wcast-qual     # cannot use because gthread.h is not complaint
    -Wmissing-prototypes
    -std=c99
    -pedantic-errors
    )

ADD_SUBDIRECTORY( src bin )
