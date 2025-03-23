from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p
import json

data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessNameAndID.restype = c_char_p


class Process:
    def __init__(self, name, PID):
        self.name = name
        self.PID = PID

    def __str__(self):
        return f"name: {self.name}, PID: {self.PID}"

# Рендеринг страницы
def show_page(request):
    processes = data.PrintProcessNameAndID().decode('utf-8')
    switch = True
    name = ""
    PID = ""
    response = dict()
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
                response[process.PID] = {
                    'name': process.name,
                    'PID': process.PID
                }

    return render(request, 'messages_app/messages.html', {'processes': response})

# Обработка AJAX-запроса
def ajax_messages(request):
    raw_data = data.PrintProcessNameAndID().decode('utf-8').strip()
    response = {'processes': {}}
    
    # Разделяем процессы по ';'
    process_entries = raw_data.split(';')
    
    for entry in process_entries:
        if not entry:
            continue  # Пропускаем пустые строки
        # Разделяем имя и PID по ':'
        parts = entry.split(':')
        if len(parts) != 2:
            continue  # Некорректный формат
        name, pid = parts[0].strip(), parts[1].strip()
        response['processes'][pid] = {'name': name, 'PID': pid}

    return JsonResponse(response)