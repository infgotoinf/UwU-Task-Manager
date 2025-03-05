from django.shortcuts import render
from .models import Message
from .forms import MessageForm


# Create your views here.
def get_all_mesages(request):
    messages = Message.objects.all()
    return render(request, 'messages_app/messages.html', {'messages': messages} )

def send_message(request):
    if request.method == 'POST':
        form = MessageForm(request.POST)
        if form.is_valid():
            form.save()  # Сохраняем форму в базе данных
            return render(request, 'messages_app/send_message.html', {'form': form, 'repeat': True})
    else:
        form = MessageForm()

    return render(request, 'messages_app/send_message.html', {'form': form})

#...Ранее добавленные импорты и методы

from django.http import JsonResponse

def ajax_messages(request):
    messages = Message.objects.all() #Получим все сообщения из модели

    #Обработаем модель
    messages_data = []
    for message in messages:
        messages_data.append({
            'sender_name': message.sender_name,
            'message': message.message,
            'sent_at': message.sent_at.strftime('%Y-%m-%d %H:%M:%S'),  # Преобразуем дату в строку
        })
    return JsonResponse({'messages': messages_data})