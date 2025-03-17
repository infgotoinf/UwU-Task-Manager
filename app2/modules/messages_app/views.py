from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p
import json

# data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessNameAndID.restype = c_char_p

    
class Process:
    def __init__(self, name, PID):
        self.name = name
        self.PID = PID


# Рендеринг страницы
def show_page(request):
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
                process = (json.loads(json.dumps(Process(name, PID), default=lambda x: x.__dict__)))
                responce.update({process["PID"] : process})
                switch = True
                name = ""
                PID = ""

    return render(request, 'messages_app/messages.html', responce)

# Обработка AJAX-запроса
def ajax_messages(request):
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
                process = (json.loads(json.dumps(Process(name, PID), default=lambda x: x.__dict__)))
                responce.update({process["PID"] : process})
                switch = True
                name = ""
                PID = ""
    return JsonResponse(responce)