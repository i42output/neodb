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
#include <neodb/page.hpp>
#include <neodb/i_record.hpp>

namespace neodb
{
    enum class field_type
    {
        Datum,
        PrimaryKey,
        ForeignKey
    };

    class i_field_spec : public neolib::i_reference_counted
    {
    public:
        typedef i_field_spec abstract_type;
    public:
        virtual ~i_field_spec() = default;
    public:
        virtual void clone(neolib::i_ref_ptr<i_field_spec>& aSpec) const = 0;
    public:
        virtual i_string const& name() const = 0;
        virtual neodb::field_type field_type() const = 0;
        virtual neodb::data_type data_type() const = 0;
        virtual std::size_t layout() const = 0;
        // helpers
    public:
        neolib::ref_ptr<i_field_spec> clone() const
        {
            neolib::ref_ptr<i_field_spec> result;
            clone(result);
            return result;
        }
    };

    template <typename Interface = i_field_spec>
    class basic_field_spec : public neolib::reference_counted<Interface>
    {
    public:
        basic_field_spec(string const& aName, neodb::field_type aFieldType, neodb::data_type aDataType, std::size_t aLayout) :
            iName{ aName }, iFieldType{ aFieldType }, iDataType{ aDataType }, iLayout{ aLayout }
        {
        }
    public:
        string const& name() const final
        {
            return iName;
        }
        neodb::field_type field_type() const final
        {
            return iFieldType;
        }
        neodb::data_type data_type() const final
        {
            return iDataType;
        }
        std::size_t layout() const final
        {
            return iLayout;
        }
    private:
        string iName;
        neodb::field_type iFieldType;
        neodb::data_type iDataType;
        std::size_t iLayout;
    };

    typedef basic_field_spec<> field_spec;

    template <typename T>
    class datum_spec : public field_spec
    {
        typedef field_spec base_type;
    public:
        datum_spec(string const& aName) :
            field_spec{ aName, field_type::Datum, as_data_type_v<T>, layout_v<T> }
        {
        }
    public:
        using base_type::clone;
        void clone(neolib::i_ref_ptr<i_field_spec>& aSpec) const final
        {
            aSpec = neolib::make_ref<datum_spec<T>>(*this);
        }
    };

    template <typename T>
    class primary_key_spec : public field_spec
    {
        typedef field_spec base_type;
    public:
        primary_key_spec(string const& aName) :
            field_spec{ aName, field_type::PrimaryKey, as_data_type_v<T>, layout_v<T> }
        {
        }
    public:
        using base_type::clone;
        void clone(neolib::i_ref_ptr<i_field_spec>& aSpec) const final
        {
            aSpec = neolib::make_ref<primary_key_spec<T>>(*this);
        }
    };

    class i_foreign_key_spec : public i_field_spec
    {
    public:
        typedef i_foreign_key_spec abstract_type;
    public:
        virtual i_foreign_key_reference const& reference() const = 0;
    };

    template <typename T>
    class foreign_key_spec : public basic_field_spec<i_foreign_key_spec>
    {
        typedef basic_field_spec<i_foreign_key_spec> base_type;
    public:
        template <typename... Args>
        foreign_key_spec(string const& aName, Args&&... aReferenceArgs) :
            base_type{ aName, field_type::ForeignKey, as_data_type_v<T>, layout_v<T> }, iReference{ std::forward<Args>(aReferenceArgs)... }
        {}
    public:
        foreign_key_reference const& reference() const final
        {
            return iReference;
        }
    public:
        using base_type::clone;
        void clone(neolib::i_ref_ptr<i_field_spec>& aSpec) const final
        {
            aSpec = neolib::make_ref<foreign_key_spec<T>>(*this);
        }
    private:
        foreign_key_reference iReference;
    };

    class i_schema
    {
    public:
        typedef i_schema abstract_type;
    public:
        virtual ~i_schema() = default;
    public:
        virtual i_string const& name() const = 0;
        virtual i_vector<neolib::i_ref_ptr<i_field_spec>> const& fields() const = 0;
    };

    inline std::size_t schema_record_size(i_schema const& aSchema)
    {
        // todo
        return 42;
    }

    inline i_record& operator<<(i_record& aRecord, i_schema const& aSchema)
    {
        aRecord << "wibble"_s;
        return aRecord;
    }
}