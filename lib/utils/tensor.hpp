/* SPDX-License-Identifier: MIT */
/**
 * @file tensor.hpp
 *
 * One of the attributes the tensors are classified is their type.
 * Tensor of type (k, l) is the tensor with k contravariant indices
 * and l covariant indices. k + l gives the total order of the tensor.
 * We will stict with this notation throughout this implementation.
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

#ifndef _TENSOR_HPP_
#define _TENSOR_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <string>
#include <sstream>
#include <array>
#include <utility>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <type_traits>

/*===========================================================================*\
 * project header files
\*===========================================================================*/
#include "dimensions.hpp"
#include "span.hpp"

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/

/*===========================================================================*\
 * global type definitions
\*===========================================================================*/
namespace lts
{

template<typename T, typename K, typename L>
struct tensor
{
    using element_type = T;
    using value_type = typename std::remove_cv<T>::type;

    static constexpr std::size_t order = K::order + L::order;
    static constexpr std::size_t size = K::size * L::size;

    constexpr tensor()
    {
    }

    constexpr tensor(span<const T, size> span)
    {
        std::copy(span.begin(), span.end(), std::begin(m_array));
    }

    constexpr tensor(const tensor& other)
    {
        std::copy(std::begin(other.m_array), std::end(other.m_array), std::begin(m_array));
    }

    constexpr tensor& operator = (const tensor& other)
    {
        std::copy(std::begin(other.m_array), std::end(other.m_array), std::begin(m_array));
        return *this;
    }

    constexpr void init(span<const T, size> span)
    {
        std::copy(span.begin(), span.end(), std::begin(m_array));
    }

    constexpr bool operator == (const tensor& t) const
    {
        return std::equal(std::begin(m_array), std::end(m_array), std::begin(t.m_array));
    }

    constexpr bool operator != (const tensor& t) const
    {
        return !(*this == t);
    }

    template<std::size_t... I>
    constexpr auto* subtensor(std::index_sequence<I...>)
    {
        static_assert(sizeof...(I) > 0, "You cannot reduce dimension of a scalar");
        static_assert(sizeof...(I) <= order, "Number of dimension indices cannot be greater than tensor's order");

        using SUB_K = typename std::conditional<sizeof...(I) < K::order, dimensions_tail<K, K::order - sizeof...(I)>, dimensions<>>::type;
        using SUB_L = typename std::conditional<sizeof...(I) < K::order, L, dimensions_tail<L, order - sizeof...(I)>>::type;
        using KL = dimensions_cat<K, L>;

        std::size_t i = 0;
        std::size_t n = 0;
        ((n += KL::o[i++] * I), ...);

        return new (&m_array[n]) tensor<T, SUB_K, SUB_L>{};
    }

    constexpr auto subtensor(span<const typename K::index_type, K::order> i,
                             span<const typename L::index_type, L::order> j) const
    {
        static_assert(order == 2);
        static_assert(std::is_same<K, L>::value,
            "Number of sumation terms must be the same for contravariant and covariant indices");

        tensor<T, dimensions<K::d[0] - 1>, dimensions<L::d[0] - 1>> retval;

        for (typename K::index_type k = 0, sub_k = 0; k < K::d[0]; ++k) {
            if (k != i[0]) {
                for (typename L::index_type l = 0, sub_l = 0; l < L::d[0]; ++l) {
                    if (l != j[0]) {
                        retval.set({sub_k}, {sub_l}, m_kl[k][l]);
                        sub_l++;
                    }
                }
                sub_k++;
            }
        }

        return retval;
    }

    constexpr T sign(span<const typename K::index_type, K::order> k_indices,
                     span<const typename L::index_type, L::order> l_indices) const
    {
        std::size_t k = std::accumulate(k_indices.begin(), k_indices.end(), 0);
        std::size_t l = std::accumulate(l_indices.begin(), l_indices.end(), 0);

        return (k + l) % 2 ? -1 : +1;
    }

    constexpr T& operator () (span<const typename K::index_type, K::order> k_indices,
                              span<const typename L::index_type, L::order> l_indices)
    {
        std::size_t k = std::inner_product(k_indices.begin(), k_indices.end(), std::begin(K::o), 0);
        std::size_t l = std::inner_product(l_indices.begin(), l_indices.end(), std::begin(L::o), 0);

        return m_kl[k][l];
    }

    constexpr T get(span<const typename K::index_type, K::order> k_indices,
                    span<const typename L::index_type, L::order> l_indices) const
    {
        std::size_t k = std::inner_product(k_indices.begin(), k_indices.end(), std::begin(K::o), 0);
        std::size_t l = std::inner_product(l_indices.begin(), l_indices.end(), std::begin(L::o), 0);

        return m_kl[k][l];
    }

    constexpr void set(span<const typename K::index_type, K::order> k_indices,
                       span<const typename L::index_type, L::order> l_indices, T value)
    {
        std::size_t k = std::inner_product(k_indices.begin(), k_indices.end(), std::begin(K::o), 0);
        std::size_t l = std::inner_product(l_indices.begin(), l_indices.end(), std::begin(L::o), 0);

        m_kl[k][l] = value;
    }

    constexpr T det() const
    {
        static_assert(order == 2, "We can calculate determinant only for 2nd order tensors (matrices)");
        static_assert(std::is_same<K, L>::value,
            "Number of sumation terms must be the same for contravariant and covariant indices");

        if constexpr (K::d[0] == 1)
            return m_kl[0][0];
        else
        if constexpr (K::d[0] == 2)
            return m_kl[0][0] * m_kl[1][1] - m_kl[0][1] * m_kl[1][0];
        else {
            T det = 0;

            for (typename K::index_type k = 0; k < K::d[0]; ++k)
                det += sign({k}, {0}) * m_kl[k][0] * subtensor({k}, {0}).det();

            return det;
        }
    }

    constexpr std::string to_string(T value) const
    {
        char buf[32];

        snprintf(buf, sizeof(buf), "%+.15e", value);

        return std::string(buf);
    }

    constexpr std::ostringstream& to_string(std::ostringstream& stream, std::size_t k) const
    {
        const T* values = &m_kl[k][0];

        stream << "[";

        std::size_t l = 0;
        for (; l < (L::size - 1); ++l)
            stream << to_string(values[l]) << ", ";
        stream << to_string(values[l]);

        stream << "]";

        return stream;
    }

    constexpr std::string to_string() const
    {
        std::ostringstream stream;

        stream << "dimensions: " << K::to_string() << L::to_string() << std::endl;

        std::size_t k = 0;
        for (; k < (K::size - 1); ++k)
            to_string(stream, k) << std::endl;
        to_string(stream, k);

        return stream.str();
    }

    constexpr operator std::string() const
    {
        return to_string();
    }

    constexpr tensor& operator += (const tensor& t)
    {
        for (std::size_t n = 0; n < size; ++n)
            m_array[n] += t.m_array[n];
        return *this;
    }

    constexpr tensor& operator -= (const tensor& t)
    {
        for (std::size_t n = 0; n < size; ++n)
            m_array[n] -= t.m_array[n];
        return *this;
    }

    constexpr tensor& operator *= (const T& s)
    {
        for (std::size_t n = 0; n < size; ++n)
            m_array[n] *= s;
        return *this;
    }

    constexpr tensor& operator /= (const T& s)
    {
        for (std::size_t n = 0; n < size; ++n)
            m_array[n] /= s;
        return *this;
    }

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> operator + (const tensor<T1, K1, L1>& t1, const tensor<T1, K1, L1>& t2);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> operator - (const tensor<T1, K1, L1>& t1, const tensor<T1, K1, L1>& t2);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> operator * (const T1& s, const tensor<T1, K1, L1>& t);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> operator * (const tensor<T1, K1, L1>& t, const T1& s);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> operator / (const tensor<T1, K1, L1>& t, const T1& s);

    template<typename T1, typename K1, typename L1, typename K2, typename L2>
    constexpr friend auto operator * (const tensor<T1, K1, L1>& t1, const tensor<T1, K2, L2>& t2);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, L1, K1> transpose(const tensor<T1, K1, L1>& t);

    template<typename T1, typename K1, typename L1>
    constexpr friend tensor<T1, K1, L1> inverse(const tensor<T1, K1, L1>& t, T1 det);

private:
    union {
        T m_array[size];
        T m_kl[K::size][L::size];
    };
};

} /* end of namespace lts */

/*===========================================================================*\
 * inline function/variable definitions
\*===========================================================================*/
namespace lts
{

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> operator + (const tensor<T1, K1, L1>& t1, const tensor<T1, K1, L1>& t2)
{
    tensor<T1, K1, L1> retval;

    for (std::size_t n = 0; n < retval.size; ++n)
        retval.m_array[n] = t1.m_array[n] + t2.m_array[n];

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> operator - (const tensor<T1, K1, L1>& t1, const tensor<T1, K1, L1>& t2)
{
    tensor<T1, K1, L1> retval;

    for (std::size_t n = 0; n < retval.size; ++n)
        retval.m_array[n] = t1.m_array[n] - t2.m_array[n];

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> operator * (const T1& s, const tensor<T1, K1, L1>& t)
{
    tensor<T1, K1, L1> retval;

    for (std::size_t n = 0; n < retval.size; ++n)
        retval.m_array[n] = s * t.m_array[n];

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> operator * (const tensor<T1, K1, L1>& t, const T1& s)
{
    tensor<T1, K1, L1> retval;

    for (std::size_t n = 0; n < retval.size; ++n)
        retval.m_array[n] = t.m_array[n] * s;

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> operator / (const tensor<T1, K1, L1>& t, const T1& s)
{
    tensor<T1, K1, L1> retval;

    for (std::size_t n = 0; n < retval.size; ++n)
        retval.m_array[n] = t.m_array[n] / s;

    return retval;
}

template<typename T1, typename K1, typename L1, typename K2, typename L2>
constexpr inline auto operator * (const tensor<T1, K1, L1>& t1, const tensor<T1, K2, L2>& t2)
{
    static_assert(L1::order > 0, "No sumation indices");
    static_assert(std::is_same<L1, K2>::value,
        "Number of sumation terms must be the same for each of the sumation indices");

    using K = K1;
    using L = L2;
    using S = L1;

    tensor<T1, K, L> retval;

    K::for_each([&](const auto& k_indices) {
        L::for_each([&](const auto& l_indices) {
            T1 u = 0;
            S::for_each([&](const auto& s_indices) {
                u += t1.get(k_indices, s_indices) * t2.get(s_indices, l_indices);
            });
            retval.set(k_indices, l_indices, u);
        });
    });

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, L1, K1> transpose(const tensor<T1, K1, L1>& t)
{
    tensor<T1, L1, K1> retval;

    if constexpr ((K1::order == 0) || (L1::order == 0))
        std::copy(std::begin(t.m_array), std::end(t.m_array), std::begin(retval.m_array));
    else
        K1::for_each([&](const auto& k_indices) {
            L1::for_each([&](const auto& l_indices) {
                retval.set(l_indices, k_indices, t.get(k_indices, l_indices));
            });
        });

    return retval;
}

template<typename T1, typename K1, typename L1>
constexpr inline tensor<T1, K1, L1> inverse(const tensor<T1, K1, L1>& t, T1 det)
{
    static_assert(std::is_same<K1, L1>::value,
        "Number of sumation terms must be the same for contravariant and covariant indices");

    tensor<T1, K1, L1> retval;

    K1::for_each([&](const auto& k_indices) {
        L1::for_each([&](const auto& l_indices) {
            T1 c = t.sign(k_indices, l_indices) * t.subtensor(k_indices, l_indices).det() / det;
            retval.set(l_indices, k_indices, c);
        });
    });

    return retval;
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

#include "tensor0.hpp" /* This is partial specialization for 0 order tensors (scalars) */

#endif /* _TENSOR_HPP_ */
