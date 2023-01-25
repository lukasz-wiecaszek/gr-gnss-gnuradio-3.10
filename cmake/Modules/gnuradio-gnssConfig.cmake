find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_GNSS gnuradio-gnss)

FIND_PATH(
    GR_GNSS_INCLUDE_DIRS
    NAMES gnuradio/gnss/api.h
    HINTS $ENV{GNSS_DIR}/include
        ${PC_GNSS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_GNSS_LIBRARIES
    NAMES gnuradio-gnss
    HINTS $ENV{GNSS_DIR}/lib
        ${PC_GNSS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-gnssTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_GNSS DEFAULT_MSG GR_GNSS_LIBRARIES GR_GNSS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_GNSS_LIBRARIES GR_GNSS_INCLUDE_DIRS)
