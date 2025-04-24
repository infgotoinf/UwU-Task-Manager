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
#include <tchar.h>

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

bool ProcessHasAccess(HANDLE hProcess)
{
    FILETIME ct, et, kt, ut;
    return GetProcessTimes(hProcess, &ct, &et, &kt, &ut);
}

bool IsKnownProcess(const std::wstring& processName)
{
    std::vector<std::wstring> knownProcesses = { L"chrome", L"notepad", L"edge", L"word", };

    for (const auto& known : knownProcesses) {
        if (processName.find(known) != std::wstring::npos) {
            return true;
        }
    }
    return false;
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

    int validProcessCount = 0;

    for (int i = 0; i < processCount; i++) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (!hProcess) continue;

        TCHAR processName[MAX_PATH];
        HMODULE hMod;
        DWORD cbNeededMod;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
            if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR))) {
                std::wstring processNameW(processName);
                if (IsKnownProcess(processNameW) && ProcessHasAccess(hProcess)) {
                    PROCESS_MEMORY_COUNTERS pmc;
                    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
                        totalMemoryUsed += pmc.WorkingSetSize;

                    FILETIME ct, et, kt, ut;
                    if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut))
                        totalProcTime1 += (*(ULONGLONG*)&kt) + (*(ULONGLONG*)&ut);

                    validProcessCount++;
                }
            }
        }

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

        TCHAR processName[MAX_PATH];
        HMODULE hMod;
        DWORD cbNeededMod;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
            if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR))) {
                std::wstring processNameW(processName);
                if (IsKnownProcess(processNameW) && ProcessHasAccess(hProcess)) {
                    FILETIME ct, et, kt, ut;
                    if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut))
                        totalProcTime2 += (*(ULONGLONG*)&kt) + (*(ULONGLONG*)&ut);
                }
            }
        }

        CloseHandle(hProcess);
    }

    double cpuUsage = 0;
    if (sysTimeDiff != 0)
        cpuUsage = ((totalProcTime2 - totalProcTime1) * 100.0) / sysTimeDiff;

    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);

    double memUsage = (totalMemoryUsed * 100.0) / mem.ullTotalPhys;

    int gpuPercent = 0;
    try {
        gpuPercent = static_cast<int>(std::round(std::stod(GetGpuUsage())));
    }
    catch (...) {
        gpuPercent = 0;
    }

    int memPercent = static_cast<int>(std::round(memUsage));
    if (memPercent > 100) memPercent = 100;
    int cpuPercent = (cpuUsage >= 0 && cpuUsage <= 100) ? static_cast<int>(std::round(cpuUsage)) : 0;

    std::ostringstream oss;
    oss << validProcessCount << ":" << memPercent << ":" << cpuPercent << ":" << gpuPercent;

    result = oss.str();
    return result.c_str();
}

int main()
{
    while (true) {
        std::cout << GetSystemUsageInfo() << '\n';
    }
    return 0;
}