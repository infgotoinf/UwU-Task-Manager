#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <array>
#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

std::string GetGpuUsage()
{
    std::string result;
    std::array<char, 128> buffer;

    std::string command =
        "powershell -Command \""
        "(Get-Counter '\\GPU Engine(*)\\Utilization Percentage').CounterSamples | "
        "Where-Object {$_.InstanceName -notlike '*_Copy'} | "
        "Measure-Object -Property CookedValue -Sum | "
        "Select -ExpandProperty Sum\"";

    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) return "0";

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(0, result.find_first_not_of(" \t"));
    result.erase(result.find_last_not_of(" \t") + 1);

    return result.empty() ? "0" : result;
}

const char* GetSystemUsageInfo()
{
    static std::string result;

    DWORD processes[1024], cbNeeded;
    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
        return "Ошибка: EnumProcesses";

    int processCount = cbNeeded / sizeof(DWORD);
    SIZE_T totalMemoryUsed = 0;
    ULONGLONG totalProcTime1 = 0;

    FILETIME idle1, kernel1, user1;
    GetSystemTimes(&idle1, &kernel1, &user1);
    ULONGLONG sysTime1 = (*(ULONGLONG*)&kernel1) + (*(ULONGLONG*)&user1);

    for (int i = 0; i < processCount; i++) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (!hProcess) continue;

        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            totalMemoryUsed += pmc.WorkingSetSize;

        FILETIME ct, et, kt, ut;
        if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut))
            totalProcTime1 += (*(ULONGLONG*)&kt) + (*(ULONGLONG*)&ut);

        CloseHandle(hProcess);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    FILETIME idle2, kernel2, user2;
    GetSystemTimes(&idle2, &kernel2, &user2);
    ULONGLONG sysTime2 = (*(ULONGLONG*)&kernel2) + (*(ULONGLONG*)&user2);
    ULONGLONG sysTimeDiff = sysTime2 - sysTime1;

    ULONGLONG totalProcTime2 = 0;
    for (int i = 0; i < processCount; i++) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);
        if (!hProcess) continue;

        FILETIME ct, et, kt, ut;
        if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut))
            totalProcTime2 += (*(ULONGLONG*)&kt) + (*(ULONGLONG*)&ut);

        CloseHandle(hProcess);
    }

    double cpuUsage = ((totalProcTime2 - totalProcTime1) * 100.0) / sysTimeDiff;

    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);
    double memUsage = (totalMemoryUsed * 100.0) / mem.ullTotalPhys;

    std::string gpuUsage = GetGpuUsage();

    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed;
    oss << "Total Processes: " << processCount
        << " | RAM: " << memUsage << "% | CPU: " << cpuUsage << "% | GPU: " << gpuUsage << "%";

    result = oss.str();
    return result.c_str();
}

const char* PrintProcessInfo()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
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
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        FILETIME ct, et, kt, ut;
        if (hProcess && GetProcessTimes(hProcess, &ct, &et, &kt, &ut)) {
            procKernelTimes1[processID] = kt;
            procUserTimes1[processID] = ut;
        }
        if (hProcess) CloseHandle(hProcess);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    FILETIME kernelTime2, userTime2;
    GetSystemTimes(&idleTime, &kernelTime2, &userTime2);

    for (DWORD i = 0; i < cProcesses; i++)
    {
        DWORD processID = aProcesses[i];
        if (processID == 0) continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (!hProcess) continue;

        wchar_t szProcessName[MAX_PATH] = L"<unknown>";
        HMODULE hMod;
        DWORD cbNeededModule;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
            GetModuleBaseNameW(hProcess, hMod, szProcessName, MAX_PATH);

        PROCESS_MEMORY_COUNTERS pmc;
        double memMb = 0.0;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            memMb = std::floor((float)pmc.WorkingSetSize / 104857.6) / 10.0;

        FILETIME ct, et, kt, ut;
        double cpu = 0.0;
        if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut)) {
            if (procKernelTimes1.find(processID) != procKernelTimes1.end()) {
                ULONGLONG kernelDiff = (*(ULONGLONG*)&kt - *(ULONGLONG*)&procKernelTimes1[processID]);
                ULONGLONG userDiff = (*(ULONGLONG*)&ut - *(ULONGLONG*)&procUserTimes1[processID]);
                ULONGLONG procTotal = kernelDiff + userDiff;

                ULONGLONG sysTotal = (*(ULONGLONG*)&kernelTime2 - *(ULONGLONG*)&kernelTime1) +
                    (*(ULONGLONG*)&userTime2 - *(ULONGLONG*)&userTime1);

                cpu = (procTotal * 100.0) / sysTotal;
                cpu = std::floor(cpu * 10) / 10.0;
            }
        }

        CloseHandle(hProcess);

        int size = WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, nullptr, 0, nullptr, nullptr);
        std::string processName(size > 0 ? size : 1, 0);
        if (size > 0)
            WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, &processName[0], size, nullptr, nullptr);
        else
            processName = "<unknown>";

        std::ostringstream oss;
        oss.precision(1);
        oss << std::fixed;
        oss << memMb << "MB:" << cpu << "%:" << processName << ":" << processID << ";\n";
        result += oss.str();
    }

    char* output = new char[result.size() + 1];
    strcpy_s(output, result.size() + 1, result.c_str());
    return output;
}

int main()
{
    std::cout << GetSystemUsageInfo() << "\n\n";
    std::cout << PrintProcessInfo();
    return 0;
}