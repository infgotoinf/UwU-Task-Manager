#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <vector>
#include <string>

std::string GetMemoryInfo()
{
    std::ostringstream oss;
    DWORD processes[1024], cbNeeded, count;

    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
        return "Ошибка: Не удалось получить список процессов.";

    count = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < count; i++)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (!hProcess)
        {
            oss << "Процесс ID: " << processes[i] << " - Ошибка: " << GetLastError() << "\n";
            continue;
        }

        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
        {
            oss << "Процесс ID: " << processes[i] << "\n"
                << "\tPageFaultCount: " << pmc.PageFaultCount << "\n"
                << "\tPeakWorkingSetSize: " << floor((float)pmc.PeakWorkingSetSize / 104857.6) / 10 << " мб\n"
                << "\tWorkingSetSize: " << floor((float)pmc.WorkingSetSize / 104857.6) / 10         << " мб\n"
                << "\tPagefileUsage: " << floor((float)pmc.PagefileUsage / 104857.6) / 10           << " мб\n"
                << "\tPeakPagefileUsage: " << floor((float)pmc.PeakPagefileUsage / 104857.6) / 10   << " мб\n\n";
        }

        CloseHandle(hProcess);
    }

    return oss.str();
}

int main()
{
    setlocale(LC_ALL, "");
    std::string info = GetMemoryInfo();
    printf("%s", info.c_str());
    return 0;
}