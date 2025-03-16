from ctypes import *
from ctypes import util

dll = CDLL("E:\\Python\\uwu-task-manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# dll = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")
# dll = CDLL("C:\\Users\\studentcoll\\Desktop\\UwU\\uwu task manager2\\dll\\Dll1\\x64\\Debug\\Dll1.dll")

# func = getattr(dll, "PrintProcessNameAndID")
# func.restype = c_char_p

foo = dll.PrintProcessNameAndID()
func = c_char_p(foo).value



libc = CDLL(util.find_library('c'))
dll.PrintProcessNameAndID.restype = c_char_p
libc.free.argtypes = (c_void_p)

def hello():
    _result = dll.PrintProcessNameAndID()
    result = _result.value
    libc.free(_result)
    return result

# do the deed
print (hello())