## uwu-task-manager2
# Что нужно сделать, чтобы запустить сайт?
1. Удалить виртуальное окружение env, если оно не удалено, создать новое: в папке проекта прописать ```python -m venv env```
2. Активировать виртуальное окружение ```.\env\scripts\activate```
3. Установить django ```pip install django```
4. Запустите сайт ```cd app2``` ```python manage.py runserver```
   - Если сайт не запускается, тк не может найти python,did not find executable at стоит пересоздать проект ```django-admin startproject app3```
   - Также стоит изменить путь к DLL во views.py
# В скором времени мы планируем перейти на electron
- https://www.electronjs.org/docs/latest/tutorial/tutorial-publishing-updating
# Документация
https://docs.google.com/document/d/1wzy9-3ueceu0ePr4X_ktbAeUFNvp5a3fH7E9tbh-wcI/edit?hl=ru&tab=t.0#heading=h.r7q27l8e5x4k
