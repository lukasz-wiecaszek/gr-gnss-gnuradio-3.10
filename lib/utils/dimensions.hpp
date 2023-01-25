/* SPDX-License-Identifier: MIT */
/**
 * @file dimensions.hpp
 *
 * 'dimensions' is a type representing both contravariant and covariant
 * dimensions of a tensor.
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

#ifndef _DIMENSIONS_HPP_
#define _DIMENSIONS_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <cstddef>
#include <array>
#include <string>
#include <sstream>
#include <utility>
#include <functional>

/*===========================================================================*\
 * project header files
\*===========================================================================*/

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/

/*===========================================================================*\
 * global type definitions
\*===========================================================================*/
namespace lts
{

/*===========================================================================*\
 * isequence
\*===========================================================================*/
template<std::size_t First, typename Sequence>
struct __isequence;

template<std::size_t First, std::size_t... Indices>
struct __isequence<First, std::index_sequence<Indices...>>
{
    using type = std::index_sequence<First + Indices...>;
};

template<std::size_t First, std::size_t Count>
using isequence = typename __isequence<First, std::make_index_sequence<Count>>::type;

/*===========================================================================*\
 * dimensions
\*===========================================================================*/
template<std::size_t... Is>
struct dimensions
{
    using index_type = int;
    using array = std::array<index_type, sizeof...(Is)>;

    static constexpr std::size_t order = sizeof...(Is);
    static constexpr array d = {Is...};

    static constexpr std::size_t index(index_type value)
    {
        for (std::size_t o = 0; o < order; ++o)
            if (d[o] == value)
                return o;
        return static_cast<std::size_t>(-1);
    }

    static constexpr std::size_t N(std::size_t index)
    {
        index_type n = 1;
        for (std::size_t o = 0; o <= index; ++o)
            n *= d[o];
        return n;
    }

    static constexpr std::size_t O(std::size_t index)
    {
        index_type n = 1;
        for (std::size_t o = index + 1; o < order; ++o)
            n *= d[o];
        return n;
    }

    static constexpr void for_each(const std::function<void(const array& indices)>& action)
    {
        array indices;

        if constexpr (order > 0) {
            std::function<void(std::size_t)> f = [&](std::size_t i) {
                if (i < (order - 1))
                    for (indices[i] = 0; indices[i] < d[i]; ++indices[i])
                        f(i + 1);
                else
                    for (indices[i] = 0; indices[i] < d[i]; ++indices[i])
                        action(indices);
            };
            f(0);
        }
        else
            action(indices);
    }

    static constexpr std::string to_string()
    {
        std::ostringstream stream;
        stream << "[";

        for (std::size_t o = 0; o < order; ++o) {
            stream << d[o];
            if (o < (order - 1))
                stream << ", ";
        }

        stream << "]";
        return stream.str();
    }

    static constexpr array n = {N(index(Is))...};
    static constexpr array o = {O(index(Is))...};
    static constexpr std::size_t size = (order == 0) ? 1 : n[order - 1];
};

/*===========================================================================*\
 * dimensions_subset, dimensions_head, dimensions_tail
\*===========================================================================*/
template<typename Dimensions, typename Sequence>
struct dimensions_subset;

template <std::size_t... Is, std::size_t... Indices>
struct dimensions_subset<dimensions<Is...>, std::index_sequence<Indices...>>
{
    using type = dimensions<dimensions<Is...>::d[Indices]...>;
};

template <typename Dimensions, std::size_t Count>
using dimensions_head = typename dimensions_subset<Dimensions, isequence<0, Count>>::type;

template <typename Dimensions, std::size_t Count>
using dimensions_tail = typename dimensions_subset<Dimensions, isequence<Dimensions::order - Count, Count>>::type;

/*===========================================================================*\
 * dimensions_cat
\*===========================================================================*/
template<typename Dimensions1, typename Sequence1, typename Dimensions2, typename Sequence2>
struct __dimensions_cat;

template <std::size_t... Is1, std::size_t... Indices1, std::size_t... Is2, std::size_t... Indices2>
struct __dimensions_cat<dimensions<Is1...>, std::index_sequence<Indices1...>, dimensions<Is2...>, std::index_sequence<Indices2...>>
{
    using type = dimensions<dimensions<Is1...>::d[Indices1]..., dimensions<Is2...>::d[Indices2]...>;
};

template <typename Dimensions1, typename Dimensions2>
using dimensions_cat = typename __dimensions_cat<
    Dimensions1, std::make_index_sequence<Dimensions1::order>,
    Dimensions2, std::make_index_sequence<Dimensions2::order>>::type;

} /* end of namespace lts */

/*===========================================================================*\
 * inline function/variable definitions
\*===========================================================================*/
namespace lts
{

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

#endif /* _DIMENSIONS_HPP_ */
