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
#include <Windows.h>

using Therena::LaunchBounce::Logger;
using Therena::LaunchBounce::Process;
using Therena::LaunchBounce::Configuration;

int wmain(int argc, wchar_t* argv[])
{
    Configuration::Initialize(argc, argv);

    Logger::Info(L"##################################################################################");
    Logger::Info(L"Execute launch bounce");
    Logger::Info(L"##################################################################################");

    const auto result = Process::Launch();

    if (ERROR_SUCCESS != result)
    {
        Logger::Error(L"Execution finished with error code: " + std::to_wstring(result));
    }
    else
    {
        Logger::Info(L"Execution finished successfully");
    }

    return result;
}

