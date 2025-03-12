# views.py
from django.shortcuts import render
from django.http import JsonResponse
from ctypes import CDLL
import sys

num = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")

# Рендеринг страницы
def show_page(request):
    return render(request, 'messages_app/messages.html')

# Обработка AJAX-запроса
def ajax_messages(request):
    number = num.PrintProcessNameAndID()
    return JsonResponse({'number': number})