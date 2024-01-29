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
#include "Process.h"
#include "Configuration.h"
#include <chrono>

using Therena::LaunchBounce::Logger;
using Therena::LaunchBounce::Process;
using Therena::LaunchBounce::StandardPipe;
using Therena::LaunchBounce::Configuration;

Process::Process(const PROCESS_INFORMATION& processInformation,
    const std::shared_ptr<StandardPipe>& errorPipe,
    const std::shared_ptr<StandardPipe>& outputPipe)
    : m_ProcessInformation(processInformation),
    m_ErrorPipe(errorPipe),
    m_OutputPipe(outputPipe)
{

}

Process::~Process()
{
    CloseHandle(m_ProcessInformation.hThread);
    CloseHandle(m_ProcessInformation.hProcess);
}

void Process::Wait()
{
    std::thread processOutputReader(&Process::ReadProcessPipes, this);
    processOutputReader.join();
}

void Process::PrintData(std::vector<unsigned char> data)
{
    const auto totalWchars = MultiByteToWideChar(CP_UTF7, 0, reinterpret_cast<char*>(data.data()), static_cast<int>(data.size()), nullptr, 0);
    if (totalWchars == 0)
    {
        return;
    }

    std::vector<wchar_t> buffer(totalWchars, L'\0');
    MultiByteToWideChar(CP_UTF7, 0, reinterpret_cast<char*>(data.data()), static_cast<int>(data.size()), buffer.data(), totalWchars);

    std::wstring logMessage;
    for (uint32_t i = 0; i != buffer.size(); ++i)
    {
        if (L'\r' == buffer[i] || L'\n' == buffer[i])
        {
            if (false == logMessage.empty())
            {
                Logger::Info(logMessage);
                logMessage.clear();
            }
        }
        else if(L'\0' != buffer[i])
        {
            logMessage += buffer[i];
        }
    }

    if (false == logMessage.empty())
    {
        Logger::Info(logMessage);
        logMessage.clear();
    }
}

std::vector<unsigned char> Process::ReadPipe()
{
    constexpr DWORD bufferSize = 1024;

    DWORD dataAvailable = 0;
    auto result = PeekNamedPipe(m_OutputPipe->GetRead(), nullptr, 0, nullptr, &dataAvailable, nullptr);
    if (FALSE == result)
    {
        return std::vector<unsigned char>();
    }

    if (0 == dataAvailable)
    {
        return std::vector<unsigned char>();
    }

    DWORD dataRead = 0;
    std::vector<unsigned char> buffer(bufferSize, L'\0');
    result = ReadFile(m_OutputPipe->GetRead(), buffer.data(), static_cast<DWORD>(buffer.size()), &dataRead, nullptr);
    if (FALSE == result)
    {
        return std::vector<unsigned char>();
    }

    return buffer;
}

void Process::ReadProcessPipes()
{
    Logger::Info(L"----------------------------------------------------------------------------------");

    while (IsRunning())
    {
        const auto result = ReadPipe();
        if (0 != result.size())
        {
            PrintData(result);
        }

        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    const auto result = ReadPipe();
    if (0 != result.size())
    {
        PrintData(result);
    }

    Logger::Info(L"----------------------------------------------------------------------------------");
}

bool Process::IsRunning()
{
    return WAIT_TIMEOUT ==
        WaitForSingleObject(m_ProcessInformation.hProcess, 0);
}

int Process::GetExitCode() const
{
    DWORD exitCode = EXIT_FAILURE;
    if (FALSE == GetExitCodeProcess(m_ProcessInformation.hProcess, &exitCode))
    {
        return EXIT_FAILURE;
    }

    return exitCode;
}

int Process::Launch()
{
    std::wstring processFile;
    const auto processResult = Configuration::GetParameter(Configuration::ParameterType::Process, processFile);
    if (false == processResult || false == std::filesystem::exists(processFile))
    {
        return ERROR_FILE_NOT_FOUND;
    }

    std::wstring cmdLine = L"\"" + processFile + L"\"";

    std::wstring parameter;
    const auto parameterResult = Configuration::GetParameter(Configuration::ParameterType::Parameter, parameter);
    if (parameterResult)
    {
        cmdLine += L" " + parameter;
    }

    Logger::Info(std::format(L"Launch the application: {}", cmdLine));

    SECURITY_ATTRIBUTES processAttributes{};
    SECURITY_ATTRIBUTES threadAttributes{};

    const auto errorPipe = std::make_shared<StandardPipe>(L"StandardError");
    const auto outputPipe = std::make_shared<StandardPipe>(L"StandardOutput");

    STARTUPINFO startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.hStdOutput = outputPipe->GetWrite();
    startupInfo.hStdError = errorPipe->GetWrite();
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION processInformation{};

    const auto processCreation = CreateProcess(nullptr,
        const_cast<LPWSTR>(cmdLine.c_str()),
        &processAttributes,
        &threadAttributes,
        true,
        NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
        nullptr,
        nullptr,
        &startupInfo,
        &processInformation);

    Process process(processInformation, errorPipe, outputPipe);

    if (FALSE == processCreation)
    {
        return GetLastError();
    }

    process.Wait();

    return process.GetExitCode();
}