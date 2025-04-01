#include "pch.h"
#include <utility>
#include "TaskManager.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

const char* PrintProcessInfo() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    DWORD processID;
    std::string result;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return "";

    cProcesses = cbNeeded / sizeof(DWORD);

    FILETIME idleTime, kernelTime1, userTime1;
    GetSystemTimes(&idleTime, &kernelTime1, &userTime1);

    std::map<DWORD, FILETIME> procKernelTimes1, procUserTimes1;

    for (DWORD i = 0; i < cProcesses; i++)
    {
        DWORD processID = aProcesses[i];
        if (processID == 0) continue;

        wchar_t szProcessName[MAX_PATH] = L"<unknown>";
        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE,
            processID
        );
        FILETIME procCreationTime, procExitTime, procKernelTime, procUserTime;
        if (GetProcessTimes(hProcess, &procCreationTime, &procExitTime, &procKernelTime, &procUserTime)) {
            procKernelTimes1[aProcesses[i]] = procKernelTime;
            procUserTimes1[aProcesses[i]] = procUserTime;
        }
        CloseHandle(hProcess);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    FILETIME kernelTime2, userTime2;
    GetSystemTimes(&idleTime, &kernelTime2, &userTime2);

    for (DWORD i = 0; i < cProcesses; i++)
    {
        DWORD processID = aProcesses[i];
        if (processID == 0) continue;

        wchar_t szProcessName[MAX_PATH] = L"<unknown>";
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

        if (hProcess != NULL)
        {
            HMODULE hMod;
            DWORD cbNeededModule;
            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
            {
                GetModuleBaseNameW(hProcess, hMod, szProcessName,
                    sizeof(szProcessName) / sizeof(wchar_t)
                );
            }
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            {
                result += std::to_string(floor((float)pmc.WorkingSetSize / 104857.6) / 10);
                result.erase(result.find_last_not_of('0') + 1, std::string::npos);
                result.erase(result.find_last_not_of('.') + 1, std::string::npos);
                result += ":";
            }

            FILETIME procCreationTime, procExitTime, procKernelTime2, procUserTime2;
            if (GetProcessTimes(hProcess, &procCreationTime, &procExitTime, &procKernelTime2, &procUserTime2)) {
                if (procKernelTimes1.find(processID) == procKernelTimes1.end()) continue;

                ULONGLONG kernelDiff = (*(ULONGLONG*)&procKernelTime2 - *(ULONGLONG*)&procKernelTimes1[processID]);
                ULONGLONG userDiff = (*(ULONGLONG*)&procUserTime2 - *(ULONGLONG*)&procUserTimes1[processID]);
                ULONGLONG procTotal = kernelDiff + userDiff;

                ULONGLONG sysTotal = (*(ULONGLONG*)&kernelTime2 - *(ULONGLONG*)&kernelTime1) +
                    (*(ULONGLONG*)&userTime2 - *(ULONGLONG*)&userTime1);

                double cpuUsage = (procTotal * 100.0) / sysTotal;
                result += std::to_string(floor(cpuUsage * 10) / 10);
                result.erase(result.find_last_not_of('0') + 1, std::string::npos);
                result.erase(result.find_last_not_of('.') + 1, std::string::npos);
                result += ":";
            }
            CloseHandle(hProcess);
        }
        else continue;

        // Конвертация из UTF-16 в UTF-8
        int size = WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, nullptr, 0, nullptr, nullptr);
        std::string processName;
        if (size > 0)
        {
            std::vector<char> buffer(size);
            WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, buffer.data(), size, nullptr, nullptr);
            processName = buffer.data();
        }
        else processName = "<unknown>";

        result += processName + ":" + std::to_string(processID) + ";";
    }

    char* output = new char[result.size() + 1];
    strcpy_s(output, result.size() + 1, result.c_str());
    return output;
}



bool KillProcessByPID(int pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        return false;
    }

    if (!TerminateProcess(hProcess, 0)) {
        CloseHandle(hProcess);
        return false;
    }
    return true;
}