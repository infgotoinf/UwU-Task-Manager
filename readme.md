## uwu-task-manager2
# Что нужно сделать, чтобы запустить сайт?
1. Удалить виртуальное окружение env, если оно не удалено, создать новое: в папке проекта прописать ```python -m venv env```
2. Активировать виртуальное окружение ```.\env\scripts\activate```
3. Установить django ```pip install django```
4. Запустите сайт ```cd app2``` ```python manage.py runserver```
   - Если сайт не запускается, тк не может найти python,did not find executable at стоит пересоздать проект ```django-admin startproject app3```
