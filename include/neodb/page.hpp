/*
 *  Copyright (c) 2021 Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/buffers.hpp>
#include <boost/endian/conversion.hpp>

using namespace boost::endian;

namespace neodb
{
    template <typename Pointer = little_uint64_t>
    struct basic_page_link
    {
        typedef Pointer pointer_type;
        typedef pointer_type size_type;

        pointer_type previous;
        pointer_type next;
        size_type used;
    };

    template <typename Pointer = little_uint64_t>
    struct basic_page_header
    {
        typedef Pointer pointer_type;
        typedef basic_page_link<pointer_type> link_type;

        link_type pageLink;
        link_type recordLink;
    };

    typedef little_uint64_t magic_t;
    magic_t const MAGIC = 0x31307642444F454E; // NEODBv01

    struct bad_magic : std::runtime_error { bad_magic() : std::runtime_error{ "neodb::bad_magic" } {} };

    template <typename Pointer = little_uint64_t>
    struct basic_root_page_header
    {
        typedef Pointer pointer_type;
        typedef basic_page_link<pointer_type> link_type;

        magic_t magic = MAGIC;
        link_type freePages;
        link_type tableSchemaPages;
        link_type tablePages;
        link_type indexPages;
    };

    template <typename Header = basic_page_header<>, std::size_t Size = 8192>
    struct basic_page
    {
        static constexpr std::size_t size = Size;

        typedef Header header_type;
        typedef typename header_type::pointer_type pointer_type;
        typedef std::array<std::uint8_t, Size - sizeof(header_type)> data_type;

        header_type header;
        data_type data;

        void clear()
        {
            header = header_type{};
            data = data_type{};
        }
        template <typename T>
        T const* data_as() const
        {
            return reinterpret_cast<T const*>(&data[0]);
        }
        template <typename T>
        T* data_as()
        {
            return reinterpret_cast<T*>(&data[0]);
        }
        template <typename T>
        void emplace(T const& aData)
        {
            new (data_as<char>()) T{ aData };
        }
        template <typename T>
        T const& as() const
        {
            return *reinterpret_cast<T const*>(data_as<char>());
        }
        template <typename T>
        T& as()
        {
            return *reinterpret_cast<T const*>(data_as<char>());
        }
    };

    using root_page = basic_page<basic_root_page_header<>>;
    using page = basic_page<>;

    template <typename Char, typename CharT, typename T>
    inline void endian_write(std::basic_ostream<Char, CharT>& aStream, T const& aEndianBuffer)
    {
        aStream.write(reinterpret_cast<char const*>(aEndianBuffer.data()), sizeof(aEndianBuffer));
    }
        
    template <typename Char, typename CharT, typename T>
    inline void endian_read(std::basic_istream<Char, CharT>& aStream, T& aEndianBuffer)
    {
        aStream.read(reinterpret_cast<char*>(aEndianBuffer.data()), sizeof(aEndianBuffer));
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_page_link<Pointer> const& aLink)
    {
        endian_write(aStream, aLink.previous);
        endian_write(aStream, aLink.next);
        endian_write(aStream, aLink.used);
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_page_header<Pointer> const& aHeader)
    {
        aStream << aHeader.pageLink;
        aStream << aHeader.recordLink;
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_root_page_header<Pointer> const& aRootHeader)
    {
        endian_write(aStream, aRootHeader.magic);
        aStream << aRootHeader.freePages;
        aStream << aRootHeader.tableSchemaPages;
        aStream << aRootHeader.tablePages;
        aStream << aRootHeader.indexPages;
        return aStream;
    }

    template <typename Char, typename CharT, typename Header, std::size_t Size>
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_page<Header, Size> const& aPage)
    {
        aStream << aPage.header;
        aStream.write(aPage.data_as<char>(), aPage.data.size());
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_page_link<Pointer>& aLink)
    {
        endian_read(aStream, aLink.previous);
        endian_read(aStream, aLink.next);
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_page_header<Pointer>& aHeader)
    {
        aStream >> aHeader.pageLink;
        aStream >> aHeader.recordLink;
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_root_page_header<Pointer>& aRootHeader)
    {
        endian_read(aStream, aRootHeader.magic);
        if (aRootHeader.magic != MAGIC)
            throw bad_magic();
        aStream >> aRootHeader.freePages;
        aStream >> aRootHeader.tableSchemaPages;
        aStream >> aRootHeader.tablePages;
        aStream >> aRootHeader.indexPages;
        return aStream;
    }

    template <typename Char, typename CharT, typename Header, std::size_t Size>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_page<Header, Size>& aPage)
    {
        aStream >> aPage.header;
        aStream.read(aPage.data_as<char>(), aPage.data.size());
        return aStream;
    }
}