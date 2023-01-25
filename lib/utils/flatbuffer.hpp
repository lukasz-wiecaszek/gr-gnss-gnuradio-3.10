/**
 * @file flatbuffer.hpp
 *
 * Definition of generic flatbuffer design pattern.
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

#ifndef _FLATBUFFER_HPP_
#define _FLATBUFFER_HPP_

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <string>
#include <sstream>

#include <cstring>

#if defined(DEBUG_FLATBUFFER)
#include <iostream>
#endif

#if !defined(CACHELINE_SIZE)
#define CACHELINE_SIZE 64
#endif

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

template<typename T>
class flatbuffer
{
public:
    typedef T value_type;

    explicit flatbuffer(std::size_t capacity = 0) :
        m_capacity{capacity},
        m_counters{capacity},
        m_buffer{nullptr}
    {
        m_buffer = new T[m_capacity];

#if defined(DEBUG_FLATBUFFER)
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << to_string() << std::endl;
#endif
    }

    ~flatbuffer()
    {
#if defined(DEBUG_FLATBUFFER)
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

        delete [] m_buffer;
    }

    flatbuffer(const flatbuffer& other) = delete;

    flatbuffer(flatbuffer&& other) :
        m_capacity{std::move(other.m_capacity)},
        m_counters{std::move(other.m_counters)},
        m_buffer{std::move(other.m_buffer)}
    {
        other.m_buffer = 0;
    }

    flatbuffer& operator = (const flatbuffer& other) = delete;

    flatbuffer& operator = (flatbuffer&& other)
    {
        m_capacity = std::move(other.m_capacity);
        m_counters = std::move(other.m_counters);
        m_buffer = std::move(other.m_buffer);

        other.m_buffer = 0;

        return *this;
    }

    bool is_valid() const
    {
        return (m_capacity > 0) && (m_buffer != nullptr);
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

    std::size_t write_available() const
    {
        return m_counters.m_write_avail;
    }

    std::size_t read_available() const
    {
        return m_counters.m_read_avail;
    }

    const T* read_ptr() const
    {
        return m_buffer + m_counters.m_read_idx;
    }

    std::size_t consume(std::size_t count)
    {
        if (count > m_counters.m_read_avail)
            count = m_counters.m_read_avail;

        m_counters.m_read_idx   += count;
        m_counters.m_read_avail -= count;

        return count;
    }

    void set_bookmark()
    {
        m_counters.m_bookmark_idx = m_counters.m_read_idx;
    }

    const T* get_bookmark() const
    {
        return (m_counters.m_bookmark_idx >= 0) ? m_buffer + m_counters.m_bookmark_idx : nullptr;
    }

    void clear_bookmark()
    {
        m_counters.m_bookmark_idx = -1;
    }

    void reset()
    {
        m_counters.reset(m_capacity);
    }

    void move()
    {
        std::size_t x;

        if (m_counters.m_bookmark_idx >= 0) {
            x = m_counters.m_bookmark_idx;
            m_counters.m_bookmark_idx = 0;
        }
        else
            x = m_counters.m_read_idx;

        if (x > 0) {
            std::size_t available_elements = m_counters.m_write_idx - x;
            std::size_t offset = m_counters.m_read_idx - x;

            std::memmove((void*)m_buffer, &m_buffer[x], available_elements * sizeof(T));

            m_counters.m_read_idx = offset;
            m_counters.m_write_idx = available_elements;
            m_counters.m_write_avail = m_capacity - available_elements;
        }
    }

    std::size_t write(const T data[], std::size_t count)
    {
        if (count > m_counters.m_write_avail)
            count = m_counters.m_write_avail;

        memcpy((void*)&m_buffer[m_counters.m_write_idx], data, count * sizeof(T));

        m_counters.m_write_idx   += count;
        m_counters.m_write_avail -= count;
        m_counters.m_read_avail  += count;

        return count;
    }

    std::size_t write(const T& data)
    {
        return write(&data, 1);
    }

    template<std::size_t N>
    std::size_t write(const T (&data)[N])
    {
        return write(data, N);
    }

    std::string to_string() const
    {
        std::ostringstream stream;

        stream << "flatbuffer@";
        stream << std::hex << this;
        stream << " [capacity: ";
        stream << std::dec << m_capacity;
        stream << ", ";
        stream << static_cast<std::string>(m_counters);
        stream << "]";

        return stream.str();
    }

    operator std::string () const
    {
        return to_string();
    }

private:
    struct alignas(CACHELINE_SIZE) counters
    {
        counters(std::size_t capacity)
        {
            reset(capacity);
        }

        void reset(std::size_t capacity)
        {
            m_read_idx = 0;
            m_read_avail = 0;
            m_write_idx = 0;
            m_write_avail = capacity;
            m_bookmark_idx = -1;
        }

        std::string to_string() const
        {
            std::ostringstream stream;

            stream << "[";
            stream << "read_idx: ";
            stream << m_read_idx;
            stream << ", ";
            stream << "write_idx: ";
            stream << m_write_idx;
            stream << ", ";
            stream << "bookmark_idx: ";
            stream << m_bookmark_idx;
            stream << ", ";
            stream << "read_avail: ";
            stream << m_read_avail;
            stream << ", ";
            stream << "write_avail: ";
            stream << m_write_avail;
            stream << "]";

            return stream.str();
        }

        operator std::string () const
        {
            return to_string();
        }

        std::size_t m_read_idx;
        std::size_t m_read_avail;
        std::size_t m_write_idx;
        std::size_t m_write_avail;
        long m_bookmark_idx; /* -1 when not set */
    };

    std::size_t m_capacity;
    counters m_counters;
    T* m_buffer;
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

#endif /* _FLATBUFFER_HPP_ */
