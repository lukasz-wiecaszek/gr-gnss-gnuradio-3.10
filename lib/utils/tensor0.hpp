/* SPDX-License-Identifier: MIT */
/**
 * @file tensor0.hpp
 *
 * This is partial specialization for 0 order tensor (scalar).
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

#ifndef _TENSOR0_HPP_
#define _TENSOR0_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/

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

using D0 = dimensions<>;

template<typename T>
struct tensor<T, D0, D0>
{
    using element_type = T;
    using value_type = typename std::remove_cv<T>::type;

    static constexpr std::size_t order = 0;
    static constexpr std::size_t size = 1;

    constexpr tensor()
    {
    }

    constexpr tensor(span<const T, size> span)
    {
        m_value = span[0];
    }

    constexpr tensor(const tensor& other)
    {
        m_value = other.m_value;
    }

    constexpr tensor& operator = (const tensor& other)
    {
        m_value = other.m_value;
        return *this;
    }

    constexpr void init(span<const T, size> span)
    {
        m_value = span[0];
    }

    constexpr bool operator == (const tensor& t) const
    {
        return m_value == t.m_value;
    }

    constexpr bool operator != (const tensor& t) const
    {
        return !(*this == t);
    }

    constexpr operator T () const
    {
        return m_value;
    }

    constexpr T& operator () (span<const typename D0::index_type, 0> k_indices,
                              span<const typename D0::index_type, 0> l_indices)
    {
        return m_value;
    }

    constexpr T get(span<const typename D0::index_type, 0> k_indices,
                    span<const typename D0::index_type, 0> l_indices) const
    {
        return m_value;
    }

    constexpr void set(span<const typename D0::index_type, 0> k_indices,
                       span<const typename D0::index_type, 0> l_indices, T value)
    {
        m_value = value;
    }

    constexpr std::string to_string(T value) const
    {
        char buf[16];

        snprintf(buf, sizeof(buf), "%+e", value);

        return std::string(buf);
    }

    constexpr std::string to_string() const
    {
        std::ostringstream stream;

        stream << "dimensions: " << "[]" << "[]" << std::endl;
        stream << "[";

        stream << to_string(m_value);

        stream << "]";
        return stream.str();
    }

    constexpr operator std::string() const
    {
        return to_string();
    }

    constexpr tensor& operator += (const tensor& t)
    {
        m_value += t.m_value;
        return *this;
    }

    constexpr tensor& operator -= (const tensor& t)
    {
        m_value -= t.m_value;
        return *this;
    }

    constexpr tensor& operator *= (const T& s)
    {
        m_value *= s;
        return *this;
    }

    constexpr tensor& operator /= (const T& s)
    {
        m_value /= s;
        return *this;
    }

    template<typename T1>
    constexpr friend tensor<T1, D0, D0> operator + (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2);

    template<typename T1>
    constexpr friend tensor<T1, D0, D0> operator - (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2);

    template<typename T1>
    constexpr friend tensor<T1, D0, D0> operator * (const T1& s, const tensor<T1, D0, D0>& t);

    template<typename T1>
    constexpr friend tensor<T1, D0, D0> operator * (const tensor<T1, D0, D0>& t, const T1& s);

    template<typename T1>
    constexpr friend tensor<T1, D0, D0> operator / (const tensor<T1, D0, D0>& t, const T1& s);

    template<typename T1>
    constexpr friend auto operator * (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2);

private:
    T m_value;
};

} /* end of namespace lts */

/*===========================================================================*\
 * inline function/variable definitions
\*===========================================================================*/
namespace lts
{

template<typename T1>
constexpr inline tensor<T1, D0, D0> operator + (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2)
{
    return tensor<T1, D0, D0>{{t1.m_value + t2.m_value}};
}

template<typename T1>
constexpr inline tensor<T1, D0, D0> operator - (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2)
{
    return tensor<T1, D0, D0>{{t1.m_value - t2.m_value}};
}

template<typename T1>
constexpr inline tensor<T1, D0, D0> operator * (const T1& s, const tensor<T1, D0, D0>& t)
{
    return tensor<T1, D0, D0>{{s * t.m_value}};
}

template<typename T1>
constexpr inline tensor<T1, D0, D0> operator * (const tensor<T1, D0, D0>& t, const T1& s)
{
    return tensor<T1, D0, D0>{{t.m_value * s}};
}

template<typename T1>
constexpr inline tensor<T1, D0, D0> operator / (const tensor<T1, D0, D0>& t, const T1& s)
{
    return tensor<T1, D0, D0>{{t.m_value / s}};
}

template<typename T1>
constexpr inline auto operator * (const tensor<T1, D0, D0>& t1, const tensor<T1, D0, D0>& t2)
{
    return tensor<T1, D0, D0>{{t1.m_value * t2.m_value}};
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

#endif /* _TENSOR0_HPP_ */
