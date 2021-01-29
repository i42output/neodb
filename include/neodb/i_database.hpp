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
#include <neodb/i_table.hpp>
#include <neodb/table_schema.hpp>
#include <neodb/page.hpp>

namespace neodb
{
    class i_database
    {
    public:
        typedef i_database abstract_type;
    public:
        virtual ~i_database() = default;
    public:
        virtual i_string const& name() const = 0;
        virtual i_vector<i_ref_ptr<i_table>> const& tables() const = 0;
        virtual void create_table(i_table_schema const& aSchema) = 0;
    public:
        virtual root_page const& root() const = 0;
        virtual root_page& root() = 0;
        virtual void allocate_table_schema_page(page& aNewPage) = 0;
        virtual void allocate_table_page(page& aNewPage) = 0;
        virtual void allocate_index_page(page& aNewPage) = 0;
        virtual void free_table_schema_page(page& aPage) = 0;
        virtual void free_table_page(page& aPage) = 0;
        virtual void free_index_page(page& aPage) = 0;
    };

    inline void create_table(i_database& aDatabase, i_table_schema const& aSchema)
    {
        aDatabase.create_table(aSchema);
    }

    template <typename... Fields, typename... FieldSpecs>
    inline void create_table(i_database& aDatabase, string const& aTableName, FieldSpecs&&... aFieldSpecs)
    {
        aDatabase.create_table(typed_table_schema<Fields...>{ aTableName, std::forward<FieldSpecs>(aFieldSpecs)... });
    }
}