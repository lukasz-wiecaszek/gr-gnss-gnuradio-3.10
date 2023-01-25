/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_NAVIGATION_SYSTEM_H
#define INCLUDED_GNSS_NAVIGATION_SYSTEM_H

namespace gr {
  namespace gnss {

    typedef enum {
        NAVIGATION_SYSTEM_UNDEFINED,
        NAVIGATION_SYSTEM_GPS,
        NAVIGATION_SYSTEM_GALILEO,
        NAVIGATION_SYSTEM_GLONASS,
    } navigation_system_e;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_NAVIGATION_SYSTEM_H */

