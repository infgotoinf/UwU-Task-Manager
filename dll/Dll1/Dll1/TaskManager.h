#pragma once

#ifdef TASKMANAGER_EXPORTS
#define TASKMANAGER_API __declspec(dllexport)
#else
#define TASKMANAGER_API __declspec(dllimport)
#endif

extern "C" TASKMANAGER_API int test();
extern "C" TASKMANAGER_API const char* PrintProcessNameAndID();
extern "C" TASKMANAGER_API bool KillProcessByPID(int pid);