from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p
import json

data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# result1 = data.KillProcessByPID(11644)
# result2 = data.KillProcessByPID(69)

# print(result1)
# print(result2)

data.PrintProcessNameAndID.restype = c_char_p

# result_ptr = data.PrintProcessNameAndID()
# result_str = result_ptr.decode('utf-8')
# print(result_str)



class Process:
    def __init__(self, name, PID):
        self.name = name
        self.PID = PID

    def __str__(self):
        return f"name: {self.name}, PID: {self.PID}"

# Рендеринг страницы
def show_page():
    processes = data.PrintProcessNameAndID().decode('utf-8')
    switch = True
    name = ""
    PID = ""
    responce = dict()
    for i in processes:
        if (switch):
            if (i != ':'):
                name += i
            else:
                switch = False
        else:
            if (i != ';'):
                PID += i
            else:
                process = Process(name, PID)
                switch = True
                name = ""
                PID = ""
                responce[process.PID] = {
                    'name': process.name,
                    'PID': process.PID
                }

    return responce

getet = show_page()

print(getet)