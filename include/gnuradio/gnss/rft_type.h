/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_RFT_TYPE_H
#define INCLUDED_GNSS_RFT_TYPE_H

namespace gr {
  namespace gnss {

    typedef enum {
        RFT_UNDEFINED,

        // Geographic/Geospatial Coordinate System (GCS) to Earth Centered Earth Fixed (ECEF)
        RFT_GCS_TO_ECEF,

        // Earth Centered Earth Fixed (ECEF) to Geographic/Geospatial Coordinate System (GCS)
        RFT_ECEF_TO_GCS
    } rft_type_e;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_RFT_TYPE_H */

