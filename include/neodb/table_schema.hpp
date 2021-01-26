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
#include <tuple>
#include <string>
#include <neodb/data_type.hpp>
#include <neodb/i_table_schema.hpp>

namespace neodb
{
    template <typename T, typename SpecT>
    struct typed_field_spec : SpecT
    {
        typedef SpecT base_spec_type;

        typed_field_spec(string const& name) :
            base_spec_type{ name, data_type::Void /* todo */ }
        {
        }
        template <typename... Args>
        typed_field_spec(string const& name, Args&&... aArgs) :
            base_spec_type{ name, data_type::Void /* todo */, typename base_spec_type::extra_type{ std::forward<Args>(aArgs)... } }
        {
        }
    };

    template <typename T>
    struct to_field_spec { typedef typed_field_spec<T, field_spec> type; };
    template <typename T>
    struct to_field_spec<primary_key<T>> { typedef typed_field_spec<T, primary_key_spec> type; };
    template <typename T>
    struct to_field_spec<foreign_key<T>> { typedef typed_field_spec<T, foreign_key_spec> type; };
    template <typename T>
    using to_field_spec_t = typename to_field_spec<T>::type;

    template <typename T>
    using as_foreign_key = to_field_spec_t<foreign_key<T>>;

    class table_schema : public i_table_schema
    {
    public:
        template <typename... FieldSpecs>
        table_schema(string const& aTableName, FieldSpecs&&... aFieldSpecs) :
            iName{ aTableName }, iFields{ std::forward<FieldSpecs>(aFieldSpecs)... }
        {
        }
        table_schema(i_table_schema const& aOther) :
            iName{ aOther.name() }, iFields{ aOther.fields() }
        {
        }
    public:
        string const& name() const override
        {
            return iName;
        }
        neolib::vector<field_spec_variant> const& fields() const override
        {
            return iFields;
        }
    private:
        string iName;
        neolib::vector<field_spec_variant> iFields;
    };

    template <typename... Fields>
    class typed_table_schema : public table_schema
    {
    public:
        typed_table_schema(string const& aTableName, to_field_spec_t<Fields>&&... aFieldSpecs) :
            table_schema{ aTableName, std::forward<to_field_spec_t<Fields>>(aFieldSpecs)... }
        {
        }
    };
}