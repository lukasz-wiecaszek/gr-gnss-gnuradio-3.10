/* SPDX-License-Identifier: MIT */
/**
 * @file span.hpp
 *
 * Normally I would use the std::span but it appears that std::span lacks
 * construction from std::initializer_list.
 * And actually this is what I need. So thus this tiny (all I need) implementation.
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

#ifndef _SPAN_HPP_
#define _SPAN_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <cstddef>
#include <array>
#include <initializer_list>
#include <type_traits>

/*===========================================================================*\
 * project header files
\*===========================================================================*/

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/

/*===========================================================================*\
 * global type definitions
\*===========================================================================*/
namespace {
    template <typename, typename, typename = void>
    struct is_element_type_compatible : std::false_type {};

    template <typename T, typename E>
    struct is_element_type_compatible<
        T, E,
        typename std::enable_if<
            !std::is_same<
                typename std::remove_cv<decltype(std::data(std::declval<T>()))>::type, void>::value &&
            std::is_convertible<
                typename std::remove_pointer<decltype(std::data(std::declval<T>()))>::type (*)[], E (*)[]>::value
            >::type>
        : std::true_type {};
}

namespace lts
{

template<typename T, std::size_t Extent>
struct span
{
    using element_type = T;
    using value_type = typename std::remove_cv<T>::type;

    constexpr span() noexcept : m_ptr{nullptr} { static_assert(N == 0); }

    constexpr span(T* ptr) noexcept : m_ptr{ptr} {}

    template<typename U, std::size_t N,
        typename std::enable_if<
            (N == Extent) && is_element_type_compatible<U (&)[N], T>::value,
            int>::type = 0>
    constexpr span(U (&array)[N]) noexcept : m_ptr{array} {}

    template<typename U, std::size_t N,
        typename std::enable_if<
            (N == Extent) && is_element_type_compatible<std::array<U, N>&, T>::value,
            int>::type = 0>
    constexpr span(std::array<U, N>& array) noexcept : m_ptr{array.data()} {}

    template<typename U, std::size_t N,
        typename std::enable_if<
            (N == Extent) && is_element_type_compatible<const std::array<U, N>&, T>::value,
            int>::type = 0>
    constexpr span(const std::array<U, N>& array) noexcept : m_ptr{array.data()} {}

    template<typename U,
        typename std::enable_if<
            is_element_type_compatible<std::initializer_list<U>, T>::value,
            int>::type = 0>
    constexpr span(std::initializer_list<U> list) noexcept : m_ptr{list.begin()} {}

    constexpr std::size_t size() const noexcept { return N; }
    constexpr T* data()          const noexcept { return m_ptr; }
    constexpr T* begin()         const noexcept { return data(); }
    constexpr T* end()           const noexcept { return data() + size(); }

    constexpr T& operator [] (std::size_t idx) const { return *(data() + idx); }

    constexpr bool contains(const value_type& x) const noexcept {
        for (auto it = begin(); it < end(); ++it)
            if (*it == x)
                return true;
        return false;
    }

private:
    T* m_ptr;
    static constexpr std::size_t N = Extent;
};

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

#endif /* _SPAN_HPP_ */
