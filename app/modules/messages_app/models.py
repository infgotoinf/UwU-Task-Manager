from django.db import models

# class Message(models.Model):
#     sender_name = models.CharField(max_length=100)  # Имя отправителя
#     message = models.TextField()  # Текст сообщения
#     sent_at = models.DateTimeField(auto_now_add=True)  # Время отправки

class Number(models.Model):
    number = models.IntegerField()