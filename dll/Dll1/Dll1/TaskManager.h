#pragma once

#ifdef TASKMANAGER_EXPORTS
#define TASKMANAGER_API __declspec(dllexport)
#else
#define TASKMANAGER_API __declspec(dllimport)
#endif

extern "C" TASKMANAGER_API int test();
extern "C" TASKMANAGER_API void PrintProcessNameAndID();