#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <clocale>
#include <string>
#include <vector>

std::string PrintProcessNameAndID() {
    setlocale(LC_ALL, "");
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    std::string everything;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return "Не удалось получить список процессов";
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for (DWORD i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            DWORD processID = aProcesses[i];
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            HANDLE hProcess = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE,
                processID
            );

            if (hProcess != NULL)
            {
                HMODULE hMod;
                DWORD cbNeededModule;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule)) {
                    GetModuleBaseName(
                        hProcess,
                        hMod,
                        szProcessName,
                        sizeof(szProcessName) / sizeof(TCHAR)
                    );
                }
                CloseHandle(hProcess);
            }

            // Преобразование TCHAR в std::string
            std::string processName;
            int size = WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, nullptr, 0, nullptr, nullptr);
            if (size > 0) {
                std::vector<char> buffer(size);
                WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, buffer.data(), size, nullptr, nullptr);
                processName = buffer.data();
            }
            else {
                processName = "<unknown>";
            }
            everything += processName + " (PID: " + std::to_string(processID) + ")\n";
        }
    }
    return everything;
}

int main() {
    std::cout << PrintProcessNameAndID();
    return 0;
}