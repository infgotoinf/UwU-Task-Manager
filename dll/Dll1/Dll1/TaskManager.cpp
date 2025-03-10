#include "pch.h"
#include <utility>
#include "TaskManager.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#include <random>

int test() {
	std::random_device dev;
	std::mt19937 rnd(dev());
	std::uniform_int_distribution<unsigned> dist(1, 100);
	unsigned random = dist(rnd);
	return random;
}

void PrintProcessNameAndID() {
    setlocale(0, "");
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

            _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
            /*everything += *szProcessName + " (PID: " + processID;
            everything += ")\n";*/
            CloseHandle(hProcess);
        }
    }
}