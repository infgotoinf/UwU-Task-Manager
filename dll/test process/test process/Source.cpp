#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <clocale>
#include <iostream>

char* PrintProcessNameAndID() {
    setlocale(0, "");
    char* everything = new char[0] {};
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    !EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            DWORD processID = aProcesses[i];
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ,
                FALSE, processID);


            if (NULL != hProcess)
            {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                    &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName,
                        sizeof(szProcessName) / sizeof(TCHAR));
                }
            }

           _ultoa_s(processID, everything, sizeof(processID), 32);
            _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
            std::cout << atoi(everything);
            /*everything += *szProcessName + " (PID: " + processID;
            everything += ")\n";*/
            CloseHandle(hProcess);
        }
    }
    return everything;
}

int main(void)
{
    std::cout << PrintProcessNameAndID();
    return 0;
}