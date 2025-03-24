from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p

# data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessInfo.restype = c_char_p


class Process:
    def __init__(self, name, PID, memUse):
        self.name = name
        self.PID = PID
        self.memUse = memUse

    def __str__(self):
        return f"name: {self.name}, PID: {self.PID}, memUse: {self.memUse}"

# Рендеринг страницы
def show_page(request):
    processes = data.PrintProcessInfo().decode('utf-8')
    switch = 0
    name = ""
    PID = ""
    memUse = ""
    response = dict()
    for i in processes:
        match switch:
            case 0:
                if (i != ':'):
                    memUse += i
                else:
                    if (memUse[-1] == "."):
                        memUse = memUse[:-1]
                    switch = 1
            case 1:
                if (i != '|'):
                    name += i
                else:
                    switch = 2
            case 2:
                if (i != ';'):
                    PID += i
                else:
                    process = Process(name, PID, memUse)
                    switch = 0
                    name = ""
                    PID = ""
                    memUse = ""
                    response[process.PID] = {
                        'name': process.name,
                        'memUse' : process.memUse
                    }

    return render(request, 'messages_app/messages.html', {'processes': response})

# Обработка AJAX-запроса
def ajax_messages(request):
    processes = data.PrintProcessInfo().decode('utf-8')
    switch = 0
    name = ""
    PID = ""
    memUse = ""
    response = dict()
    for i in processes:
        match switch:
            case 0:
                if (i != ':'):
                    memUse += i
                else:
                    if (memUse[-1] == "."):
                        memUse = memUse[:-1]
                    switch = 1
            case 1:
                if (i != '|'):
                    name += i
                else:
                    switch = 2
            case 2:
                if (i != ';'):
                    PID += i
                else:
                    process = Process(name, PID, memUse)
                    switch = 0
                    name = ""
                    PID = ""
                    memUse = ""
                    response[process.PID] = {
                        'name': process.name,
                        'memUse' : process.memUse
                    }

    return JsonResponse({'processes': response})