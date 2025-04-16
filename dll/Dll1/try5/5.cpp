#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <thread>
#include <chrono>

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

    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed;
    oss << processCount << " : " << memUsage << " ; " << cpuUsage;

    result = oss.str();
    return result.c_str();
}

int main() {
    printf("%s\n", GetSystemUsageInfo());
    return 0;
}