/**
 * @file ilog2.hpp
 *
 * Integer base 2 logarithm calculation.
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 */

#ifndef _ILOG2_HPP_
#define _ILOG2_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/

/*===========================================================================*\
 * project header files
\*===========================================================================*/
#include "power_of_two.hpp"

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/

/*===========================================================================*\
 * global type definitions
\*===========================================================================*/
namespace lts
{

} /* end of namespace lts */

/*===========================================================================*\
 * inline function/variable definitions
\*===========================================================================*/
namespace lts
{

template<typename T>
constexpr int ilog2(T n)
{
    /* Because the noexcept operator always returns true for a constant expression, */
    /* it can be used to check if a particular invocation of a constexpr */
    /* takes the constant expression branch. */
    if constexpr (noexcept(n)) {
        return (n) < 2 ? 0 :
            (n) & (1ULL << 63) ? 63 :
            (n) & (1ULL << 62) ? 62 :
            (n) & (1ULL << 61) ? 61 :
            (n) & (1ULL << 60) ? 60 :
            (n) & (1ULL << 59) ? 59 :
            (n) & (1ULL << 58) ? 58 :
            (n) & (1ULL << 57) ? 57 :
            (n) & (1ULL << 56) ? 56 :
            (n) & (1ULL << 55) ? 55 :
            (n) & (1ULL << 54) ? 54 :
            (n) & (1ULL << 53) ? 53 :
            (n) & (1ULL << 52) ? 52 :
            (n) & (1ULL << 51) ? 51 :
            (n) & (1ULL << 50) ? 50 :
            (n) & (1ULL << 49) ? 49 :
            (n) & (1ULL << 48) ? 48 :
            (n) & (1ULL << 47) ? 47 :
            (n) & (1ULL << 46) ? 46 :
            (n) & (1ULL << 45) ? 45 :
            (n) & (1ULL << 44) ? 44 :
            (n) & (1ULL << 43) ? 43 :
            (n) & (1ULL << 42) ? 42 :
            (n) & (1ULL << 41) ? 41 :
            (n) & (1ULL << 40) ? 40 :
            (n) & (1ULL << 39) ? 39 :
            (n) & (1ULL << 38) ? 38 :
            (n) & (1ULL << 37) ? 37 :
            (n) & (1ULL << 36) ? 36 :
            (n) & (1ULL << 35) ? 35 :
            (n) & (1ULL << 34) ? 34 :
            (n) & (1ULL << 33) ? 33 :
            (n) & (1ULL << 32) ? 32 :
            (n) & (1ULL << 31) ? 31 :
            (n) & (1ULL << 30) ? 30 :
            (n) & (1ULL << 29) ? 29 :
            (n) & (1ULL << 28) ? 28 :
            (n) & (1ULL << 27) ? 27 :
            (n) & (1ULL << 26) ? 26 :
            (n) & (1ULL << 25) ? 25 :
            (n) & (1ULL << 24) ? 24 :
            (n) & (1ULL << 23) ? 23 :
            (n) & (1ULL << 22) ? 22 :
            (n) & (1ULL << 21) ? 21 :
            (n) & (1ULL << 20) ? 20 :
            (n) & (1ULL << 19) ? 19 :
            (n) & (1ULL << 18) ? 18 :
            (n) & (1ULL << 17) ? 17 :
            (n) & (1ULL << 16) ? 16 :
            (n) & (1ULL << 15) ? 15 :
            (n) & (1ULL << 14) ? 14 :
            (n) & (1ULL << 13) ? 13 :
            (n) & (1ULL << 12) ? 12 :
            (n) & (1ULL << 11) ? 11 :
            (n) & (1ULL << 10) ? 10 :
            (n) & (1ULL <<  9) ?  9 :
            (n) & (1ULL <<  8) ?  8 :
            (n) & (1ULL <<  7) ?  7 :
            (n) & (1ULL <<  6) ?  6 :
            (n) & (1ULL <<  5) ?  5 :
            (n) & (1ULL <<  4) ?  4 :
            (n) & (1ULL <<  3) ?  3 :
            (n) & (1ULL <<  2) ?  2 :
             1;
    }
    else {
        if (sizeof(n) <= sizeof(int))
            return fls(n) - 1;
        else
        if (sizeof(n) <= sizeof(long))
            return flsl(n) - 1;
        else
            return flsll(n) - 1;
    }
}

template<typename T>
constexpr int ilog2_rounddown(T n)
{
    return ilog2(n);
}

template<typename T>
constexpr int ilog2_roundup(T n)
{
    return is_power_of_two(n) ? ilog2(n) : ilog2(n) + 1;
}

} /* end of namespace lts */

/*===========================================================================*\
 * global object declarations
\*===========================================================================*/
namespace lts
{

} /* end of namespace lts */

/*===========================================================================*\
 * function forward declarations
\*===========================================================================*/
namespace lts
{

} /* end of namespace lts */

#endif /* _ILOG2_HPP_ */
