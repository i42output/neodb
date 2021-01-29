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

#include <filesystem>
#include <fstream>
#include <neodb/database.hpp>
#include <neodb/page.hpp>

namespace neodb
{
    class file_database : public database
    {
    public:
        file_database(std::filesystem::path const& aDatabasePath) :
            database{ aDatabasePath.filename().stem().generic_string() },
            iRoot{}
        {
            iRoot.clear();
            if (!std::filesystem::exists(aDatabasePath.parent_path()))
                std::filesystem::create_directories(aDatabasePath.parent_path());
            bool newDatabase = !std::filesystem::exists(aDatabasePath);
            iFile.emplace(aDatabasePath.generic_string(), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
            if (!file())
                throw std::runtime_error{ "Failed to open database file '" + aDatabasePath.generic_string() + "'" };
            file().seekg(0);
            file().seekp(0);
            if (newDatabase)
                file() << iRoot;
            else
                file() >> iRoot;
            if (!file())
                throw std::runtime_error{ "Failed to initialise database '" + aDatabasePath.generic_string() + "'" };
        }
    private:
        std::fstream& file()
        {
            return *iFile;
        }
        void commit();
    private:
        std::optional<std::fstream> iFile;
        root_page iRoot;
    };
}