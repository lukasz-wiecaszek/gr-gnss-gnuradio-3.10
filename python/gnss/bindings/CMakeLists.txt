# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT gnss_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT gnss_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND gnss_python_files
    navigation_system_python.cc
    acquisition_and_tracking_python.cc
    signal_normalizer_python.cc
    ca_code_domain_python.cc
    ca_code_generator_python.cc
    type_converter_python.cc
    ca_symbols_to_nav_bits_python.cc
    nav_message_decoder_python.cc
    resampler_python.cc
    gnss_channel_python.cc
    pseudoranges_decoder_python.cc
    pvt_python.cc
    rft_type_python.cc
    rft_python.cc
    skip_zeros_python.cc
    doppler_shift_python.cc
    geojson_file_sink_python.cc
    number_file_sink_python.cc
    pvt_satellite_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(gnss
   ../../..
   gr::gnss
   "${gnss_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET gnss_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gnss_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/gnss/
)

install(TARGETS gnss_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/gnss COMPONENT pythonapi)
