from ctypes import CDLL, c_char_p

data = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
data.PrintProcessInfo.restype = c_char_p

result_ptr = data.PrintProcessInfo()
result_str = result_ptr.decode('utf-8')
print(result_str)