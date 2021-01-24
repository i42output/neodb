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
    enum class page_type : uint64_t
    {
        // todo
        Free            = 0x0000000000000000,
        Root_v_0_0_1    = 0x31303062646f656e, // neodb v0.0.1
        Root            = Root_v_0_0_1,
        TableSchema     = 0x0000000000000002,
        Table           = 0x0000000000000003,
        Index           = 0x0000000000000004
    };

    template <typename Pointer = little_uint64_t>
    struct basic_file_page_link
    {
        typedef Pointer pointer_type;

        pointer_type previous;
        pointer_type next;
    };

    template <typename Pointer = little_uint64_t>
    struct basic_file_page_header
    {
        typedef Pointer pointer_type;
        typedef basic_file_page_link<Pointer> link_type;

        little_uint64_t type;
        link_type link;
    };

    template <page_type Type, typename Pointer = little_uint64_t, std::size_t Size = 8192>
    struct basic_file_page
    {
        static constexpr page_type type = Type;
        static constexpr std::size_t size = Size;

        typedef Pointer pointer_type;
        typedef basic_file_page_header<pointer_type> header_type;
        typedef std::array<std::uint8_t, Size - sizeof(header_type)> data_type;

        header_type header;
        data_type data;

        void clear()
        {
            header = header_type{ native_to_little(static_cast<uint64_t>(type)) };
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

    using free_file_page = basic_file_page<page_type::Free>;
    using root_file_page = basic_file_page<page_type::Root>;
    using table_schema_file_page = basic_file_page<page_type::TableSchema>;
    using table_file_page = basic_file_page<page_type::Table>;
    using index_file_page = basic_file_page<page_type::Index>;

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
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_file_page_link<Pointer> const& aLink)
    {
        endian_write(aStream, aLink.previous);
        endian_write(aStream, aLink.next);
        return aStream;
    }

    template <typename Char, typename CharT, page_type Type, typename Pointer, std::size_t Size>
    inline std::basic_ostream<Char, CharT>& operator<<(std::basic_ostream<Char, CharT>& aStream, basic_file_page<Type, Pointer, Size> const& aPage)
    {
        endian_write(aStream, aPage.header.type);
        aStream << aPage.header.link;
        aStream.write(aPage.data_as<char>(), aPage.data.size());
        return aStream;
    }

    template <typename Char, typename CharT, typename Pointer>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_file_page_link<Pointer>& aLink)
    {
        endian_read(aStream, aLink.previous);
        endian_read(aStream, aLink.next);
        return aStream;
    }

    template <typename Char, typename CharT, page_type Type, typename Pointer, std::size_t Size>
    inline std::basic_istream<Char, CharT>& operator>>(std::basic_istream<Char, CharT>& aStream, basic_file_page<Type, Pointer, Size>& aPage)
    {
        endian_read(aStream, aPage.header.type);
        aStream >> aPage.header.link;
        aStream.read(aPage.data_as<char>(), aPage.data.size());
        return aStream;
    }
}