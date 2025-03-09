from django.urls import path
from . import views

urlpatterns = [
    path('', views.show_page, name='main_page'),        # Главная страница
    path('get_data/', views.ajax_messages, name='ajax'), # AJAX-эндпоинт
]