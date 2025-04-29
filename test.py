from ctypes import CDLL, c_char_p

def test1():
    data = CDLL("E:\\Sites\\Collage\\uwu-task-manager2\\tauri-app2\\src-tauri\\Dll1.dll")
    data.PrintProcessInfo.restype = c_char_p

    result_ptr = data.PrintProcessInfo()
    result_str = result_ptr.decode('utf-8')
    print(result_str)

def test2():
    data = CDLL("E:\\Sites\\Collage\\uwu-task-manager2\\tauri-app2\\src-tauri\\Dll1.dll")
    result1 = data.KillProcessByPID(14372) #PID фотошопа
    result2 = data.KillProcessByPID(69) #Выдуманное PID

    print(result1)
    print(result2)

test2()