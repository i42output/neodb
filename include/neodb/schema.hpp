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
#include <neodb/i_schema.hpp>

namespace neodb
{
    template <typename T>
    struct to_field_spec { typedef datum_spec<T> type; };
    template <typename T>
    struct to_field_spec<primary_key<T>> { typedef primary_key_spec<T> type; };
    template <typename T>
    struct to_field_spec<foreign_key<T>> { typedef foreign_key_spec<T> type; };
    template <typename T>
    using to_field_spec_t = typename to_field_spec<T>::type;

    template <typename T>
    using as_foreign_key = to_field_spec_t<foreign_key<T>>;

    class schema : public i_schema
    {
    public:
        template <typename... FieldSpecs>
        schema(string const& aTableName, FieldSpecs&&... aFieldSpecs) :
            iName{ aTableName }, iFields{ aFieldSpecs.clone()...}
        {
        }
        schema(i_schema const& aOther) :
            iName{ aOther.name() }, iFields{ aOther.fields().begin(), aOther.fields().end() }
        {
        }
    public:
        string const& name() const override
        {
            return iName;
        }
        neolib::vector<neolib::ref_ptr<i_field_spec>> const& fields() const override
        {
            return iFields;
        }
    private:
        string iName;
        neolib::vector<neolib::ref_ptr<i_field_spec>> iFields;
    };

    template <typename... Fields>
    class typed_schema : public schema
    {
    public:
        typed_schema(string const& aTableName, to_field_spec_t<Fields>&&... aFieldSpecs) :
            schema{ aTableName, std::forward<to_field_spec_t<Fields>>(aFieldSpecs)... }
        {
        }
    };
}