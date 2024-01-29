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
#include <format>
#include <chrono>
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "Configuration.h"

using Therena::LaunchBounce::Logger;
using Therena::LaunchBounce::Configuration;

void Logger::Info(const std::wstring& message)
{
    std::unique_lock lock(GetMutex());
    int i = 1;
    WriteLog(GetTime() + L" [INFO] [" + std::format(L"{:0>6}", GetCurrentThreadId()) + L"] " + message);
}

void Logger::Warning(const std::wstring& message)
{
    std::unique_lock lock(GetMutex());
    WriteLog(GetTime() + L" [WARN] [" + std::format(L"{:0>6}", GetCurrentThreadId()) + L"] " + message);
}

void Logger::Error(const std::wstring& message)
{
    std::unique_lock lock(GetMutex());
    WriteLog(GetTime() + L" [ERROR] [" + std::format(L"{:0>6}", GetCurrentThreadId()) + L"] " + message);
}


void Logger::WriteLog(const std::wstring& message)
{
    std::wcout << message << std::endl;

    const auto logFilePath = Configuration::GetAppPath().replace_extension(L".log");

    std::wofstream fileStream(logFilePath, std::ios_base::app);

    fileStream << message << std::endl;

    fileStream.flush();
    fileStream.close();
}

std::wstring Logger::GetTime()
{
    const auto currentTime = std::chrono::system_clock::now();
    return std::format(L"{:%Y-%m-%d %X}", currentTime);
}

std::mutex& Logger::GetMutex()
{
    static std::mutex mutex;
    return mutex;
}