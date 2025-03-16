from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL, c_char_p

data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# data = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessNameAndID.restype = c_char_p

# Рендеринг страницы
def show_page(request):
    processes = data.PrintProcessNameAndID()
    return render(request, 'messages_app/messages.html', {'processes': processes})

# Обработка AJAX-запроса
def ajax_messages(request):
    processes = data.PrintProcessNameAndID().decode('utf-8')
    lol = ""
    for i in processes:
        if (i == ':'): break
        else: lol = lol + i
    return JsonResponse({'processes': processes})