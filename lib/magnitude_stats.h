/* -*- c++ -*- */
/*
 * Copyright 2021 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GNSS_MAGNITUDE_STATS_H
#define INCLUDED_GNSS_MAGNITUDE_STATS_H

namespace gr {
  namespace gnss {

    template<typename T>
    struct magnitude_stats
    {
      magnitude_stats(int freq_index = -1)
      {
        reset(freq_index);
      }

      void reset(int freq_index = -1)
      {
        d_freq_index = freq_index;
        d_max_index = -1;
        d_min = 0.0;
        d_max = 0.0;
        d_avg = 0.0;
      }

      std::string to_string() const
      {
        char buf[128];

        snprintf(buf, sizeof(buf), "idx: %d, freq_idx: %d, min: %e, max: %e, avg: %e",
          d_max_index, d_freq_index, d_min, d_max, d_avg);

        return std::string(buf);
      }

      static magnitude_stats get(const std::vector<T>& v, int freq_index)
      {
        magnitude_stats stats{freq_index};

        if (v.size() > 0) {
          stats.d_max_index = 0;
          stats.d_min = stats.d_max = stats.d_avg = v[0];

          for (std::size_t i = 1; i < v.size(); ++i) {
            if (v[i] < stats.d_min) {
              stats.d_min = v[i];
            }
            if (v[i] > stats.d_max) {
              stats.d_max = v[i];
              stats.d_max_index = i;
            }
            stats.d_avg += v[i];
          }

          if (v.size() > 1) {
            stats.d_avg -= stats.d_max;
            stats.d_avg /= (v.size() - 1);
          }
        }

        return stats;
      }

      int d_freq_index;
      int d_max_index;
      T d_min;
      T d_max;
      T d_avg;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_MAGNITUDE_STATS_H */

