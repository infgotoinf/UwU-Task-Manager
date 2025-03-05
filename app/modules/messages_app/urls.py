from django.urls import path
from . import views

urlpatterns = [
    path('', views.get_all_mesages, name='get_all_messages'),
    path('send/', views.send_message, name='send_message'),

    #ajax
    path('get_data/', views.ajax_messages, name='get_data')
]