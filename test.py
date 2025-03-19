from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p
import json

# data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessNameAndID.restype = c_char_p

# # Указываем, что функция возвращает указатель на строку (char*)
# dll.PrintProcessNameAndID.restype = c_char_p

# def get_process_info():
#     # Вызываем функцию и получаем указатель на строку
#     result_ptr = dll.PrintProcessNameAndID()
    
#     if not result_ptr:
#         return "Ошибка: функция вернула пустой указатель"
    
#     # Преобразуем C-строку в Python строку
#     result_str = result_ptr.decode('utf-8')

#     return result_str

# # Выводим результат
# print(get_process_info())



class Process:
    def __init__(self, name, PID):
        self.name = name
        self.PID = PID

# Рендеринг страницы
def show_page():
    processes = data.PrintProcessNameAndID().decode('utf-8')
    switch = True
    name = ""
    PID = ""
    responce = []
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
                process = (json.loads(json.dumps(Process(name, PID), default=lambda x: x.__dict__)))
                responce.update({process["PID"] : process})
                switch = True
                name = ""
                PID = ""
                for process in processes:
                    responce.append({
                        'name': process.name,
                        'PID': process.PID,
                    })

    return responce

getet = show_page()

for i in getet:
    print(getet[i]["PID"])