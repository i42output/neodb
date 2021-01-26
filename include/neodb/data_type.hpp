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

#include <vector>
#include <optional>
#include <variant>
#include <chrono>
#include <neolib/core/reference_counted.hpp>
#include <neolib/core/optional.hpp>
#include <neolib/core/vector.hpp>
#include <neolib/core/variant.hpp>
#include <neolib/core/string.hpp>
#include <neolib/core/uuid.hpp>

using namespace std::string_literals;
using namespace neolib::string_literals;

namespace neodb
{
    using neolib::i_ref_ptr;
    using neolib::ref_ptr;
    using neolib::i_vector;
    using neolib::vector;
    using neolib::i_variant;
    using neolib::variant;
    using neolib::i_optional;
    using neolib::optional;
    using neolib::i_string;
    using neolib::string;
    using neolib::uuid;
    using time = std::chrono::system_clock::time_point;
    using blob = neolib::vector<uint8_t>;

    using data_value_type = variant<
        bool,
        int8_t, int16_t, int32_t, int64_t,
        uint8_t, uint16_t, uint32_t, uint64_t,
        float, double,
        char, string,
        uuid,
        time,
        blob,
        neolib::optional<bool>,
        neolib::optional<int8_t>, neolib::optional<int16_t>, neolib::optional<int32_t>, neolib::optional<int64_t>,
        neolib::optional<uint8_t>, neolib::optional<uint16_t>, neolib::optional<uint32_t>, neolib::optional<uint64_t>,
        neolib::optional<float>, neolib::optional<double>,
        neolib::optional<char>, neolib::optional<string>,
        neolib::optional<uuid>,
        neolib::optional<time>,
        neolib::optional<blob>>;

    enum data_type : uint32_t
    {
        Void            = 0x00000000,
        Bool            = 0x00000001,
        Int8            = 0x00000002,
        Int16           = 0x00000003,
        Int32           = 0x00000004,
        Int64           = 0x00000005,
        Uint8           = 0x00000006,
        Uint16          = 0x00000007,
        Uint32          = 0x00000008,
        Uint64          = 0x00000009,
        Float           = 0x00000010,
        Double          = 0x00000011,
        Char            = 0x00000012,
        String          = 0x00000013,
        Uuid            = 0x00000014,
        Time            = 0x00000015,
        Blob            = 0x00000016,
        NullableBool    = 0x00000017,
        NullableInt8    = 0x00000018,
        NullableInt16   = 0x00000019,
        NullableInt32   = 0x00000020,
        NullableInt64   = 0x00000021,
        NullableUint8   = 0x00000022,
        NullableUint16  = 0x00000023,
        NullableUint32  = 0x00000024,
        NullableUint64  = 0x00000025,
        NullableFloat   = 0x00000026,
        NullableDouble  = 0x00000027,
        NullableChar    = 0x00000028,
        NullableString  = 0x00000029,
        NullableUuid    = 0x00000030,
        NullableTime    = 0x00000031,
        NullableBlob    = 0x00000032
    };

    template <typename T>
    struct primary_key {};

    class i_foreign_key_reference
    {
    public:
        typedef i_foreign_key_reference abstract_type;
    public:
        virtual ~i_foreign_key_reference() = default;
    public:
        virtual i_string const& table() const = 0;
        virtual i_string const& field() const = 0;
    };

    class foreign_key_reference : public i_foreign_key_reference
    {
    public:
        foreign_key_reference(string const& aTable, string const& aField) :
            iTable{ aTable }, iField{ aField }
        {}
    public:
        i_string const& table() const
        {
            return iTable;
        }
        i_string const& field() const
        {
            return iField;
        }
    private:
        string iTable;
        string iField;
    };

    template <typename T>
    struct foreign_key { foreign_key_reference reference; };
}