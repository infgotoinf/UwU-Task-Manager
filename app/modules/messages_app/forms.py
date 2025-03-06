from django import forms
from .models import Number

# class MessageForm(forms.ModelForm):
#     class Meta:
#         model = Message
#         fields = ['sender_name', 'message']

class NumberForm(forms.ModelForm):
    class Meta:
        number = ['number']