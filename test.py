from ctypes import *
from ctypes.util import find_library

dll = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# dll = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")

# Указываем, что функция возвращает указатель на строку (char*)
dll.PrintProcessNameAndID.restype = c_char_p

def get_process_info():
    # Вызываем функцию и получаем указатель на строку
    result_ptr = dll.PrintProcessNameAndID()
    
    if not result_ptr:
        return "Ошибка: функция вернула пустой указатель"
    
    # Преобразуем C-строку в Python строку
    result_str = result_ptr.decode('utf-8')

    return result_str

# Выводим результат
print(get_process_info())