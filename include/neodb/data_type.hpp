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
#include <boost/endian/arithmetic.hpp>
#include <neolib/core/reference_counted.hpp>
#include <neolib/core/optional.hpp>
#include <neolib/core/vector.hpp>
#include <neolib/core/variant.hpp>
#include <neolib/core/string.hpp>
#include <neolib/core/uuid.hpp>

using namespace std::string_literals;
using namespace neolib::string_literals;

namespace neolib
{
    namespace detail
    {
        template <>
        struct abstract_type<std::chrono::system_clock::time_point> { typedef std::chrono::system_clock::time_point type; };
    }
}

namespace neodb
{
    using namespace boost::endian;

    using neolib::i_ref_ptr;
    using neolib::ref_ptr;
    using neolib::weak_ref_ptr;
    using neolib::make_ref;
    using neolib::i_vector;
    using neolib::vector;
    using neolib::i_variant;
    using neolib::variant;
    using neolib::i_optional;
    using neolib::optional;
    using neolib::i_string;
    using neolib::string;
    using neolib::uuid;
    using neolib::optional;
    using time = std::chrono::system_clock::time_point;
    using blob = vector<uint8_t>;

    enum data_type : uint32_t
    {
        Void                    = 0x00000000,
        Bool                    = 0x00000001,
        Int8                    = 0x00000002,
        Int16                   = 0x00000003,
        Int32                   = 0x00000004,
        Int64                   = 0x00000005,
        Uint8                   = 0x00000006,
        Uint16                  = 0x00000007,
        Uint32                  = 0x00000008,
        Uint64                  = 0x00000009,
        Float                   = 0x0000000a,
        Double                  = 0x0000000b,
        Char                    = 0x0000000c,
        String                  = 0x0000000d,
        CharString              = 0x0000000e,
        VarcharString           = 0x0000000f,
        Uuid                    = 0x00000010,
        Time                    = 0x00000011,
        Blob                    = 0x00000012,
        NullableBool            = 0x00000013,
        NullableInt8            = 0x00000014,
        NullableInt16           = 0x00000015,
        NullableInt32           = 0x00000016,
        NullableInt64           = 0x00000017,
        NullableUint8           = 0x00000018,
        NullableUint16          = 0x00000019,
        NullableUint32          = 0x0000001a,
        NullableUint64          = 0x0000001c,
        NullableFloat           = 0x0000001d,
        NullableDouble          = 0x0000001e,
        NullableChar            = 0x0000001f,
        NullableString          = 0x00000020,
        NullableCharString      = 0x00000021,
        NullableVarcharString   = 0x00000022,
        NullableUuid            = 0x00000023,
        NullableTime            = 0x00000024,
        NullableBlob            = 0x00000025
    };

    template <data_type Type>
    class basic_string : public neolib::string
    {
        typedef neolib::string base_type;
    public:
        using base_type::base_type;
    };

    using c_string = basic_string<data_type::CharString>;
    using vc_string = basic_string<data_type::VarcharString>;

    using data_value_type = variant<
        bool,
        int8_t, int16_t, int32_t, int64_t,
        uint8_t, uint16_t, uint32_t, uint64_t,
        float, double,
        char, string, c_string, vc_string,
        uuid,
        time,
        blob,
        optional<bool>,
        optional<int8_t>, optional<int16_t>, optional<int32_t>, optional<int64_t>,
        optional<uint8_t>, optional<uint16_t>, optional<uint32_t>, optional<uint64_t>,
        optional<float>, optional<double>,
        optional<char>, optional<string>, optional<c_string>, optional<vc_string>,
        optional<uuid>,
        optional<time>,
        optional<blob>>;

    template <typename T> struct as_data_type;
    template <> struct as_data_type<bool> { static data_type constexpr result = data_type::Bool; };
    template <> struct as_data_type<int8_t> { static data_type constexpr result = data_type::Int8; };
    template <> struct as_data_type<int16_t> { static data_type constexpr result = data_type::Int16; };
    template <> struct as_data_type<int32_t> { static data_type constexpr result = data_type::Int32; };
    template <> struct as_data_type<int64_t> { static data_type constexpr result = data_type::Int64; };
    template <> struct as_data_type<uint8_t> { static data_type constexpr result = data_type::Uint8; };
    template <> struct as_data_type<uint16_t> { static data_type constexpr result = data_type::Uint16; };
    template <> struct as_data_type<uint32_t> { static data_type constexpr result = data_type::Uint32; };
    template <> struct as_data_type<uint64_t> { static data_type constexpr result = data_type::Uint64; };
    template <> struct as_data_type<float> { static data_type constexpr result = data_type::Float; };
    template <> struct as_data_type<double> { static data_type constexpr result = data_type::Double; };
    template <> struct as_data_type<char> { static data_type constexpr result = data_type::Char; };
    template <> struct as_data_type<string> { static data_type constexpr result = data_type::String; };
    template <> struct as_data_type<c_string> { static data_type constexpr result = data_type::CharString; };
    template <> struct as_data_type<vc_string> { static data_type constexpr result = data_type::VarcharString; };
    template <> struct as_data_type<uuid> { static data_type constexpr result = data_type::Uuid; };
    template <> struct as_data_type<time> { static data_type constexpr result = data_type::Time; };
    template <> struct as_data_type<blob> { static data_type constexpr result = data_type::Blob; };
    template <> struct as_data_type<optional<bool>> { static data_type constexpr result = data_type::NullableBool; };
    template <> struct as_data_type<optional<int8_t>> { static data_type constexpr result = data_type::NullableInt8; };
    template <> struct as_data_type<optional<int16_t>> { static data_type constexpr result = data_type::NullableInt16; };
    template <> struct as_data_type<optional<int32_t>> { static data_type constexpr result = data_type::NullableInt32; };
    template <> struct as_data_type<optional<int64_t>> { static data_type constexpr result = data_type::NullableInt64; };
    template <> struct as_data_type<optional<uint8_t>> { static data_type constexpr result = data_type::NullableUint8; };
    template <> struct as_data_type<optional<uint16_t>> { static data_type constexpr result = data_type::NullableUint16; };
    template <> struct as_data_type<optional<uint32_t>> { static data_type constexpr result = data_type::NullableUint32; };
    template <> struct as_data_type<optional<uint64_t>> { static data_type constexpr result = data_type::NullableUint64; };
    template <> struct as_data_type<optional<float>> { static data_type constexpr result = data_type::NullableFloat; };
    template <> struct as_data_type<optional<double>> { static data_type constexpr result = data_type::NullableDouble; };
    template <> struct as_data_type<optional<char>> { static data_type constexpr result = data_type::NullableChar; };
    template <> struct as_data_type<optional<string>> { static data_type constexpr result = data_type::NullableString; };
    template <> struct as_data_type<optional<c_string>> { static data_type constexpr result = data_type::NullableCharString; };
    template <> struct as_data_type<optional<vc_string>> { static data_type constexpr result = data_type::NullableVarcharString; };
    template <> struct as_data_type<optional<uuid>> { static data_type constexpr result = data_type::NullableUuid; };
    template <> struct as_data_type<optional<time>> { static data_type constexpr result = data_type::NullableTime; };
    template <> struct as_data_type<optional<blob>> { static data_type constexpr result = data_type::NullableBlob; };

    class i_char_string
    {
    public:
        virtual ~i_char_string() = default;
    public:
        virtual std::size_t layout() const = 0;
    };

    template <std::size_t N>
    class char_string : public i_char_string
    {
    public:
        std::size_t layout() const final
        {
            return N;
        }
    };

    class i_varchar_string
    {
    public:
        virtual ~i_varchar_string() = default;
    public:
        virtual std::size_t layout() const = 0;
    };

    template <std::size_t N>
    class varchar_string : public i_varchar_string
    {
    public:
        std::size_t layout() const final
        {
            return N;
        }
    };

    template <std::size_t N> struct as_data_type<char_string<N>> { static data_type constexpr result = data_type::CharString; };
    template <std::size_t N> struct as_data_type<varchar_string<N>> { static data_type constexpr result = data_type::VarcharString; };
    template <std::size_t N> struct as_data_type<optional<char_string<N>>> { static data_type constexpr result = data_type::NullableCharString; };
    template <std::size_t N> struct as_data_type<optional<varchar_string<N>>> { static data_type constexpr result = data_type::NullableVarcharString; };

    template <typename T>
    data_type constexpr as_data_type_v = as_data_type<T>::result;

    template <typename T>
    struct data_ptr { typedef void* ptr; };
    template <typename T>
    struct data_ptr<T const> { typedef void const* ptr; };

    template <typename T>
    struct buffer
    {
        T& value;
        typename data_ptr<T>::ptr data() const { return &value; }
        std::size_t size() const { return sizeof(T); }
        buffer(T& value) : value{ value } {}
    };
    template <>
    struct buffer<bool const>
    {
        uint8_t const value;
        void const* data() const { return &value; }
        std::size_t size() const { return sizeof(value); }
        buffer(bool value) : value{ value } {}
    };
    template <>
    struct buffer<bool>
    {
        uint8_t value;
        void* data() { return &value; }
        std::size_t size() const { return sizeof(value); }
        buffer(bool value) : value{ value } {}
    };
    template <order Order, typename T, std::size_t Bits, align Align>
    struct buffer<endian_arithmetic<Order, T, Bits, Align> const>
    {
        endian_arithmetic<Order, T, Bits, Align> const value;
        void const* data() const { return value.data(); }
        std::size_t size() const { return Bits / 8; }
        buffer(endian_arithmetic<Order, T, Bits, Align> const& value) : value{ value } {}
    };
    template <order Order, typename T, std::size_t Bits, align Align>
    struct buffer<endian_arithmetic<Order, T, Bits, Align>>
    {
        endian_arithmetic<Order, T, Bits, Align> value;
        void* data() { return value.data(); }
        std::size_t size() const { return Bits / 8; }
        buffer(endian_arithmetic<Order, T, Bits, Align> const& value) : value{ value } {}
    };
    template <>
    struct buffer<string const>
    {
        string const& value;
        void const* data() const { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(string const& value) : value{ value } {}
    };
    template <>
    struct buffer<string>
    {
        string& value;
        void* data() { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(string& value) : value{ value } {}
    };
    template <>
    struct buffer<c_string const>
    {
        c_string const& value;
        void const* data() const { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(c_string const& value) : value{ value } {}
    };
    template <>
    struct buffer<c_string>
    {
        c_string& value;
        void* data() { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(c_string& value) : value{ value } {}
    };
    template <>
    struct buffer<vc_string const>
    {
        vc_string const& value;
        void const* data() const { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(vc_string const& value) : value{ value } {}
    };
    template <>
    struct buffer<vc_string>
    {
        vc_string& value;
        void* data() { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(vc_string& value) : value{ value } {}
    };
    template <>
    struct buffer<blob const>
    {
        blob const& value;
        void const* data() const { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(blob const& value) : value{ value } {}
    };
    template <>
    struct buffer<blob>
    {
        blob& value;
        void* data() { return value.data(); }
        std::size_t size() const { return value.size(); }
        buffer(blob& value) : value{ value } {}
    };

    template <typename T> struct as_buffer { typedef buffer<T> buffer; };
    template <> struct as_buffer<int8_t> { typedef buffer<little_int8_t> buffer; };
    template <> struct as_buffer<int16_t> { typedef buffer<little_int16_t> buffer; };
    template <> struct as_buffer<int32_t> { typedef buffer<little_int32_t> buffer; };
    template <> struct as_buffer<int64_t> { typedef buffer<little_int64_t> buffer; };
    template <> struct as_buffer<uint8_t> { typedef buffer<little_uint8_t> buffer; };
    template <> struct as_buffer<uint16_t> { typedef buffer<little_uint16_t> buffer; };
    template <> struct as_buffer<uint32_t> { typedef buffer<little_uint32_t> buffer; };
    template <> struct as_buffer<uint64_t> { typedef buffer<little_uint64_t> buffer; };
    template <> struct as_buffer<float> { typedef buffer<little_float32_t> buffer; };
    template <> struct as_buffer<double> { typedef buffer<little_float64_t> buffer; };

    template <typename T> 
    using buffer_t = typename as_buffer<T>::buffer;

    template <typename T> struct layout { static constexpr std::size_t value = 1; };
    template <std::size_t N> struct layout<char_string<N>> { static constexpr std::size_t value = N; };
    template <std::size_t N> struct layout<varchar_string<N>> { static constexpr std::size_t value = N; };
    template <typename T>
    std::size_t constexpr layout_v = layout<T>::value;

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
        i_string const& table() const final
        {
            return iTable;
        }
        i_string const& field() const final
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