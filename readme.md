# UwU task manager
## User Guide
### Что нужно сделать, чтобы запустить ~~приложение~~ <ins>сайт</ins>?
1. Создать новое виртуальноя приложение: используя ```cd``` перейти в папку проекта и прописать ```python -m venv env```
2. Активировать виртуальное окружение ```.\env\scripts\activate```
3. Установить django ```pip install django```
4. Запустите сайт ```cd app2``` ```python manage.py runserver```
   - Если сайт не запускается, тк не может найти python,did not find executable at стоит пересоздать проект ```django-admin startproject app3``` и скопировать в него все важные файлы из предыдущего
   - Также стоит изменить путь к DLL во views.py
### <ins>Как запустить приложение?</ins>
1. ```cd tauri-app2```
2. ```bun install```
   - Если не установлен bun ```npm install -g bun```
3. ```bun run tauri dev```
### Как пользоваться нашим приложением?
Наше приложение показывает вам актуальные данные о ваших процессах, просто перемещайтесь вверх и вниз, смотрите по названию (Name) или PID процесса сколько памяти в мегабайтах оно использует (MemUse) или насколько сильно нагружает процессор (CPUUse).
### Документация
https://docs.google.com/document/d/1wzy9-3ueceu0ePr4X_ktbAeUFNvp5a3fH7E9tbh-wcI/edit?hl=ru&tab=t.0#heading=h.r7q27l8e5x4k
## ~~В скором времени мы планируем перейти на electron~~
- ~~https://www.electronjs.org/docs/latest/tutorial/tutorial-publishing-updating~~
<ins>Электрон для лохов.</ins>
