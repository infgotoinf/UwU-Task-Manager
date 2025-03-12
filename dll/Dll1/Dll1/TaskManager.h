#pragma once

#ifdef TASKMANAGER_EXPORTS
#define TASKMANAGER_API __declspec(dllexport)
#else
#define TASKMANAGER_API __declspec(dllimport)
#endif

#include <string>

extern "C" TASKMANAGER_API int test();
extern "C" TASKMANAGER_API std::string PrintProcessNameAndID();