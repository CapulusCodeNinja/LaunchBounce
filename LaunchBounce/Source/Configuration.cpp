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
#include "Logger.h"
#include "Configuration.h"
#include <Windows.h>

using Therena::LaunchBounce::Configuration;

void Configuration::Initialize(int argc, wchar_t* argv[])
{
    if (0 == argc || argc % 2 == 0)
    {
        return;
    }

    for (auto i = 1; i < argc; i = i + 2)
    {
        auto type = ParameterType::Unknown;
        const auto result = ConvertParameterType(argv[i], type);
        if (result)
        {
            m_Parameters[type] = argv[i + 1];
        }
    }
}

bool Configuration::GetParameter(ParameterType type, std::wstring& parameter)
{
    if (m_Parameters.end() == m_Parameters.find(type))
    {
        return false;
    }

    parameter = m_Parameters[type];

    return true;
}

bool Configuration::ConvertParameterType(const std::wstring& parameter, ParameterType& type)
{
    if (std::wstring::npos != parameter.find(L"Process"))
    {
        type = ParameterType::Process;
        return true;
    }

    if (std::wstring::npos != parameter.find(L"Parameter"))
    {
        type = ParameterType::Parameter;
        return true;
    }

    return false;
}

std::filesystem::path Configuration::GetAppPath()
{
    wchar_t pathBuffer[MAX_PATH + 1];
    ::GetModuleFileNameW(nullptr, pathBuffer, MAX_PATH);
    return pathBuffer;
}
