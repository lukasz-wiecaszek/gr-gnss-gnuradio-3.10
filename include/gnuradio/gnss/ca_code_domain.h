/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_CA_CODE_DOMAIN_H
#define INCLUDED_GNSS_CA_CODE_DOMAIN_H

namespace gr {
  namespace gnss {

    typedef enum {
        CA_CODE_DOMAIN_TIME,
        CA_CODE_DOMAIN_FREQUENCY,
        CA_CODE_DOMAIN_FREQUENCY_CONJUGATE,
    } ca_code_domain_e;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_DOMAIN_H */

