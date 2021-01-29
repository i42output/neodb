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
#include <neodb/i_database.hpp>
#include <neodb/table.hpp>

namespace neodb
{
    class database : public i_database
    {
    public:
        typedef vector<ref_ptr<table>> table_list;
    public:
        database(string const& aDatabaseName) :
            iName{ aDatabaseName },
            iRoot{}
        {
        }
    public:
        i_string const& name() const override
        {
            return iName;
        }
        table_list const& tables() const override
        {
            return iTables;
        }
        void create_table(i_table_schema const& aSchema) override
        {
            iTables.push_back(make_ref<table>(*this, aSchema));
        }
    public:
        root_page const& root() const override
        {
            return iRoot;
        }
        root_page& root() override
        {
            return iRoot;
        }
        void allocate_record(record_type aRecordType, link::size_type aRecordSize, i_ref_ptr<i_record>& aNewRecord) override
        {
        }
        void free_record(i_record& aExistingRecord) override
        {
        }
    protected:
        virtual page::pointer_type allocate_page() = 0;
        virtual void free_page(page::pointer_type aAddress) = 0;
    private:
        string iName;
        root_page iRoot;
        table_list iTables;
    };
}