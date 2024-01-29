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

#include <filesystem>
#include <Windows.h>
#include "StandardPipe.h"

namespace Therena::LaunchBounce
{
    class Process final
    {
    private:
        Process(const PROCESS_INFORMATION& processInformation,
            const std::shared_ptr<StandardPipe>& errorPipe,
            const std::shared_ptr<StandardPipe>& outputPipe);

        ~Process();

        std::vector<unsigned char> ReadPipe();

        void ReadProcessPipes();

        bool IsRunning();

        void Wait();

        int GetExitCode() const;

        void PrintData(std::vector<unsigned char> data);

    public:
        static int Launch();

    private:
        PROCESS_INFORMATION m_ProcessInformation{};
        std::shared_ptr<StandardPipe> m_ErrorPipe{};
        std::shared_ptr<StandardPipe> m_OutputPipe{};
    };
}
