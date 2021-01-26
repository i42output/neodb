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

#include <neolib/core/i_vector.hpp>
#include <neodb/data_type.hpp>

namespace neodb
{
    class i_field_spec
    {
    public:
        typedef i_field_spec abstract_type;
    public:
        virtual ~i_field_spec() = default;
    public:
        virtual i_string const& name() const = 0;
        virtual data_type type() const = 0;
    };

    template <typename Interface = i_field_spec>
    class basic_field_spec : public Interface
    {
    public:
        basic_field_spec(string const& aName, data_type aType) :
            iName{ aName }, iType{ aType }
        {
        }
    public:
        string const& name() const override
        {
            return iName;
        }
        data_type type() const override
        {
            return iType;
        }
    private:
        string iName;
        data_type iType;
    };

    typedef basic_field_spec<> field_spec;

    class primary_key_spec : public field_spec
    {
    public:
        using field_spec::field_spec;
    };

    class i_foreign_key_spec : public i_field_spec
    {
    public:
        typedef i_foreign_key_spec abstract_type;
    public:
        virtual i_foreign_key_reference const& reference() const = 0;
    };

    class foreign_key_spec : public basic_field_spec<i_foreign_key_spec>
    {
        typedef basic_field_spec<i_foreign_key_spec> base_type;
    public:
        typedef foreign_key_reference extra_type;
    public:
        foreign_key_spec(string const& aName, data_type aType, foreign_key_reference const& aReference) :
            base_type{ aName, aType }, iReference{ aReference }
        {}
    public:
        foreign_key_reference const& reference() const override
        {
            return iReference;
        }
    private:
        foreign_key_reference iReference;
    };

    using field_spec_variant = variant<field_spec, primary_key_spec, foreign_key_spec>;
    using i_field_spec_variant = neolib::abstract_t<field_spec_variant>;

    class i_table_schema
    {
    public:
        typedef i_table_schema abstract_type;
    public:
        virtual ~i_table_schema() = default;
    public:
        virtual i_string const& name() const = 0;
        virtual i_vector<i_field_spec_variant> const& fields() const = 0;
    };
}