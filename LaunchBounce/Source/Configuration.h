//
// Copyright 2024 David Roller 
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//  http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once

#include <map>
#include <filesystem>

namespace Therena::LaunchBounce
{
    class Configuration final
    {
    public:
        Configuration() = delete;

        static void Initialize();

        static std::filesystem::path GetAppPath();

        enum class ParameterType
        {
            Unknown,
            Process,
            Parameter,
            Hide
        };

        static bool GetParameter(ParameterType type, std::wstring& parameter);

    private:
        static bool ConvertParameterType(const std::wstring& parameter, ParameterType& type);

        static std::vector<std::pair<std::wstring, std::wstring>> ParseCommandLine(const std::wstring& commandLine);

    private:
        inline static std::map<ParameterType, std::wstring> m_Parameters{};
    };
}
