/*
 * NetBuffer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-11
 *
 */

#ifndef __NET_BUFFER_H__
#define __NET_BUFFER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class NetBuffer
    {
    public:
        typedef std::size_t size_type;
        typedef boost::asio::streambuf streambuf_type;
        typedef streambuf_type::const_buffers_type const_buffers_type;
        typedef streambuf_type::mutable_buffers_type mutable_buffers_type;

    private:
        BOOST_STATIC_CONSTANT(size_type, BUF_SIZE = 512);
        streambuf_type m_buf;

    public:
        mutable_buffers_type prepare(size_type n = BUF_SIZE)
        {
            return m_buf.prepare(n);
        }

        void retrieve(size_type n)
        {
            m_buf.commit(n);
        }

        size_type size() const
        {
            return m_buf.size();
        }

        const char *peek() const
        {
            return boost::asio::buffer_cast<const char *>(m_buf.data());
        }

        void append(const void *data, size_type len)
        {
            m_buf.sputn(static_cast<const char *>(data), boost::numeric_cast<std::streamsize>(len));
        }

        const_buffers_type data() const
        {
            return m_buf.data();
        }

        void consume(size_type n)
        { 
            m_buf.consume(n);
        }
    };

}

#endif // __NET_BUFFER_H__
