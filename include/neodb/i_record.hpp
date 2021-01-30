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

#include <neodb/data_type.hpp>

namespace neodb
{
    class i_database;

    enum class record_type
    {
        Schema,
        Table,
        Index,

        COUNT
    };

    template <typename T>
    void const* to_data(T const& aValue, std::size_t& aDataLength)
    {
        thread_local std::optional<buffer<T const>> buffer;
        buffer.emplace(aValue);
        aDataLength = buffer->size();
        return buffer->data();
    };

    class i_record : public neolib::i_reference_counted
    {
    public:
        typedef i_record abstract_type;
    public:
        virtual i_database& database() const = 0;
        virtual record_type type() const = 0;
        virtual link::size_type size() const = 0;
    public:
        virtual void write(void const* aData, std::size_t aDataLength) = 0;
        virtual void read(void* aData, std::size_t aDataLength) = 0;
    public:
        template <typename T>
        void write(T const& aValue)
        {
            std::size_t dataLength;
            auto const data = to_data(aValue, dataLength);
            write(data, dataLength);
        }
        template <typename T>
        void write(optional<T> const& aValue)
        {
            if (aValue)
            {
                write(true);
                write(*aValue);
            }
            else
                write(false);
        }
    };

    inline i_record& operator<<(i_record& aRecord, data_value_type const& aValue)
    {
        std::visit([&](auto&& aValue)
        {
            aRecord.write(aValue);
        }, aValue);
        return aRecord;
    }
}