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
#include "StandardPipe.h"
#include <Windows.h>

using Therena::LaunchBounce::StandardPipe;

StandardPipe::StandardPipe(const std::wstring& type)
    : m_Type(type)
{
    SECURITY_ATTRIBUTES securityAttributes{};

    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = nullptr;

    if (FALSE == CreatePipe(&m_Read, &m_Write, &securityAttributes, 0))
    {
        m_Read = INVALID_HANDLE_VALUE;
        m_Write = INVALID_HANDLE_VALUE;
        return;
    }

    SetHandleInformation(m_Read, HANDLE_FLAG_INHERIT, 0);
}

StandardPipe::~StandardPipe()
{
    if (nullptr != m_Read && INVALID_HANDLE_VALUE != m_Read)
    {
        CloseHandle(m_Read);
    }

    if (nullptr != m_Write && INVALID_HANDLE_VALUE != m_Write)
    {
        CloseHandle(m_Write);
    }
}

HANDLE StandardPipe::GetRead()
{
    return m_Read;
}

HANDLE StandardPipe::GetWrite()
{
    return m_Write;
}
