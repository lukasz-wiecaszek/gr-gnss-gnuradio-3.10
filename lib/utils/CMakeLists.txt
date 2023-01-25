# Copyright 2011,2012,2016,2018,2019 Free Software Foundation, Inc.
#
# This file is a part of gr-gnss
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Setup library
########################################################################
target_include_directories(gnuradio-gnss
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gnuradio-gnss
)

list(APPEND gnss_sources
)

set(gnss_sources ${gnss_sources} PARENT_SCOPE)
