#include <netdb.h> // addrinfo, getaddrinfo, freeaddrinfo
#include <cstring> // memset

int main() {
    const char* hostname = "www.baidu.com";
    const char* port = "80";

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // 允许IPv4或IPv6
    hints.ai_socktype = SOCK_STREAM;  // 流式套接字

    int status = getaddrinfo(hostname, port, &hints, &result);
    if (status != 0) {
        // 获取地址信息失败
        // 处理错误情况
        return 1;
    }

    // 遍历结果链表
    struct addrinfo* p = result;
    while (p != nullptr) {
        // 可以访问p->ai_family、p->ai_addr等成员来获取具体的信息
        p = p->ai_next;
    }

    // 释放内存
    freeaddrinfo(result);

    return 0;
}