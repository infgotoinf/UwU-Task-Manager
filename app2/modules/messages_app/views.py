from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p

# data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessInfo.restype = c_char_p


class Process:
    def __init__(self, name, PID, memUse, CPUUse):
        self.name = name
        self.PID = PID
        self.memUse = memUse
        self.CPUUse = CPUUse

def ParseProcessInfo():
    processes = data.PrintProcessInfo().decode('utf-8')
    switch = 0
    name = ""
    PID = ""
    memUse = ""
    CPUUse = ""
    response = dict()
    for i in processes:
        match switch:
            case 0:
                if (i != ':'):
                    memUse += i
                else:
                    switch = 1
            case 1:
                if (i != ':'):
                    CPUUse += i
                else:
                    switch = 2
            case 2:
                if (i != ':'):
                    name += i
                else:
                    switch = 3
            case 3:
                if (i != ';'):
                    PID += i
                else:
                    process = Process(name, PID, memUse, CPUUse)
                    switch = 0
                    name = ""
                    PID = ""
                    memUse = ""
                    CPUUse = ""
                    response[process.PID] = {
                        'name': process.name,
                        'memUse' : process.memUse,
                        'CPUUse' : process.CPUUse
                    }
    return response

# Рендеринг страницы
def show_page(request):
    return render(request, 'messages_app/messages.html', {'processes': ParseProcessInfo()})

# Обработка AJAX-запроса
def ajax_messages(request):
    return JsonResponse({'processes': ParseProcessInfo()})