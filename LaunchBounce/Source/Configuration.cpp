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

void Configuration::Initialize()
{
    const auto commandLineString = GetCommandLine();
    const auto commandLine = ParseCommandLine(commandLineString);

    for (const auto& param : commandLine)
    {
        auto type = ParameterType::Unknown;
        const auto result = ConvertParameterType(param.first, type);
        if (result)
        {
            m_Parameters[type] = param.second;
        }
    }
}

std::vector<std::pair<std::wstring, std::wstring>> Configuration::ParseCommandLine(const std::wstring& commandLine)
{
    std::vector<std::pair<std::wstring, std::wstring>> params;
    std::wistringstream iss(commandLine);
    std::wstring token;
    std::wstring key;
    std::wstring value;

    bool isFirstToken = true;
    bool isQuotedValue = false;
    while (iss >> token)
    {
        if (isFirstToken)
        {
            isFirstToken = false;
            continue;
        }

        if (isQuotedValue)
        {
            if (token.back() == L'"')
            {
                value += L" " + token.substr(0, token.length() - 1);
                isQuotedValue = false;
                params.push_back({ key, value });
                value.clear();
            }
            else
            {
                value += L" " + token;
            }
        }
        else if (token.front() == L'"')
        {
            value = token.substr(1);
            if (token.back() != L'"')
            {
                isQuotedValue = true;
            }
            else
            {
                value.pop_back();
                params.push_back({ key, value });
                value.clear();
            }
        }
        else if (token.front() == L'-' && !isQuotedValue)
        {
            key = token;
        }
        else
        {
            params.push_back({ key, token });
        }
    }

    return params;
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


    if (std::wstring::npos != parameter.find(L"Hide"))
    {
        type = ParameterType::Hide;
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
