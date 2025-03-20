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
        return "������: �� ������� �������� ������ ���������.";

    count = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < count; i++)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (!hProcess)
        {
            oss << "������� ID: " << processes[i] << " - ������: " << GetLastError() << "\n";
            continue;
        }

        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
        {
            oss << "������� ID: " << processes[i] << "\n"
                << "\tPageFaultCount: " << pmc.PageFaultCount << "\n"
                << "\tPeakWorkingSetSize: " << floor((float)pmc.PeakWorkingSetSize / 104857.6) / 10 << " ��\n"
                << "\tWorkingSetSize: " << floor((float)pmc.WorkingSetSize / 104857.6) / 10         << " ��\n"
                << "\tPagefileUsage: " << floor((float)pmc.PagefileUsage / 104857.6) / 10           << " ��\n"
                << "\tPeakPagefileUsage: " << floor((float)pmc.PeakPagefileUsage / 104857.6) / 10   << " ��\n\n";
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