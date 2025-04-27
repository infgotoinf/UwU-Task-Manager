#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <map>
#include <sstream>
#include <array>
#include <memory>
#include <regex>

std::map<DWORD, double> GetGpuUsagePerProcess() {
    std::map<DWORD, double> gpuUsageMap;
    std::string result;
    std::array<char, 512> buffer;

    std::string command =
        "powershell -Command \""
        "Get-Counter '\\GPU Engine(*)\\Utilization Percentage' | "
        "Select -ExpandProperty CounterSamples | "
        "Where-Object { $_.InstanceName -like 'pid_*' } | "
        "ForEach-Object { $_.InstanceName + '|' + $_.CookedValue }\"";

    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) return gpuUsageMap;

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    std::istringstream stream(result);
    std::string line;
    std::regex regex(R"(pid_(\d+)_.*\|([\d.]+))");

    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_search(line, match, regex)) {
            DWORD pid = std::stoi(match[1].str());
            double usage = std::stod(match[2].str());
            gpuUsageMap[pid] += usage;
        }
    }

    return gpuUsageMap;
}

const char* PrintProcessInfo() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    std::string result;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return "";

    cProcesses = cbNeeded / sizeof(DWORD);
    auto gpuUsageMap = GetGpuUsagePerProcess();

    FILETIME idleTime, kernelTime1, userTime1;
    GetSystemTimes(&idleTime, &kernelTime1, &userTime1);

    std::map<DWORD, FILETIME> procKernelTimes1, procUserTimes1;

    for (DWORD i = 0; i < cProcesses; i++) {
        DWORD processID = aProcesses[i];
        if (processID == 0) continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (!hProcess) continue;

        FILETIME ct, et, kt, ut;
        if (GetProcessTimes(hProcess, &ct, &et, &kt, &ut)) {
            procKernelTimes1[processID] = kt;
            procUserTimes1[processID] = ut;
        }
        CloseHandle(hProcess);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    FILETIME kernelTime2, userTime2;
    GetSystemTimes(&idleTime, &kernelTime2, &userTime2);

    for (DWORD i = 0; i < cProcesses; i++) {
        DWORD processID = aProcesses[i];
        if (processID == 0) continue;

        wchar_t szProcessName[MAX_PATH] = L"<unknown>";
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
        if (!hProcess) continue;

        HMODULE hMod;
        DWORD cbNeededModule;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule)) {
            GetModuleBaseNameW(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(wchar_t));
        }

        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            result += std::to_string(floor((float)pmc.WorkingSetSize / 104857.6) / 10);
            result.erase(result.find_last_not_of('0') + 1, std::string::npos);
            result.erase(result.find_last_not_of('.') + 1, std::string::npos);
            result += ":";
        }

        FILETIME ct, et, kt2, ut2;
        if (GetProcessTimes(hProcess, &ct, &et, &kt2, &ut2) &&
            procKernelTimes1.find(processID) != procKernelTimes1.end()) {

            ULONGLONG kernelDiff = (*(ULONGLONG*)&kt2 - *(ULONGLONG*)&procKernelTimes1[processID]);
            ULONGLONG userDiff = (*(ULONGLONG*)&ut2 - *(ULONGLONG*)&procUserTimes1[processID]);
            ULONGLONG procTotal = kernelDiff + userDiff;

            ULONGLONG sysTotal = (*(ULONGLONG*)&kernelTime2 - *(ULONGLONG*)&kernelTime1) +
                (*(ULONGLONG*)&userTime2 - *(ULONGLONG*)&userTime1);
            double cpuUsage = (procTotal * 100.0) / sysTotal;
            result += std::to_string(floor(cpuUsage * 10) / 10);
            result.erase(result.find_last_not_of('0') + 1, std::string::npos);
            result.erase(result.find_last_not_of('.') + 1, std::string::npos);
            result += ":";
        }

        double gpuUsage = gpuUsageMap.count(processID) ? gpuUsageMap[processID] : 0.0;
        result += std::to_string(floor(gpuUsage * 10) / 10);
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);
        result.erase(result.find_last_not_of('.') + 1, std::string::npos);
        result += ":";

        int size = WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, nullptr, 0, nullptr, nullptr);
        std::string processName;
        if (size > 0) {
            std::vector<char> buffer(size);
            WideCharToMultiByte(CP_UTF8, 0, szProcessName, -1, buffer.data(), size, nullptr, nullptr);
            processName = buffer.data();
        }
        else {
            processName = "<unknown>";
        }

        result += processName + ":" + std::to_string(processID) + ";";

        CloseHandle(hProcess);
    }

    char* output = new char[result.size() + 1];
    strcpy_s(output, result.size() + 1, result.c_str());
    return output;
}

int main() {
    std::cout << PrintProcessInfo();
}