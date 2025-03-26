#include <windows.h>
#include <iostream>
#include <thread>

double GetCPUUsage(DWORD processID) {

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processID);
    if (!hProcess) return -1;

    FILETIME idleTime, kernelTime1, userTime1, kernelTime2, userTime2;
    FILETIME procCreationTime, procExitTime, procKernelTime1, procUserTime1, procKernelTime2, procUserTime2;
    GetSystemTimes(&idleTime, &kernelTime1, &userTime1);
    GetProcessTimes(hProcess, &procCreationTime, &procExitTime, &procKernelTime1, &procUserTime1);

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Ждём 1 секунду
    GetSystemTimes(&idleTime, &kernelTime2, &userTime2);
    GetProcessTimes(hProcess, &procCreationTime, &procExitTime, &procKernelTime2, &procUserTime2);

    CloseHandle(hProcess);

    ULONGLONG sysKernelDiff = (*(ULONGLONG*)&kernelTime2 - *(ULONGLONG*)&kernelTime1);
    ULONGLONG sysUserDiff = (*(ULONGLONG*)&userTime2 - *(ULONGLONG*)&userTime1);
    ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;

    ULONGLONG procKernelDiff = (*(ULONGLONG*)&procKernelTime2 - *(ULONGLONG*)&procKernelTime1);
    ULONGLONG procUserDiff = (*(ULONGLONG*)&procUserTime2 - *(ULONGLONG*)&procUserTime1);
    ULONGLONG procTotal = procKernelDiff + procUserDiff;

    return (procTotal * 100.0) / sysTotal;
}

int main() {
    setlocale(0, "");
    DWORD pid;
    std::cout << "Введите PID процесса: ";
    std::cin >> pid;

    double cpuUsage = GetCPUUsage(pid);
    if (cpuUsage >= 0)
        std::cout << "Процесс " << pid << " использует " << cpuUsage << "% CPU\n";
    else
        std::cout << "Ошибка! Не удалось открыть процесс.\n";

    return 0;
}