#include <netdb.h> // addrinfo, getaddrinfo, freeaddrinfo
#include <cstring> // memset
#include <iostream>
using namespace std;

void print_sa_data(struct sockaddr *addr) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    unsigned char *data = (unsigned char *)addr_in->sin_zero; // 或者使用 addr_in->sin_zero，取决于具体的结构体类型

    printf("sa_data: ");
    for (int i = 0; i < sizeof(addr_in->sin_zero); i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


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
        std::cerr << "getaddrinfo fail" << std::endl;
        return 1;
    }
    cout << "getaddrinfo: " << status << endl;

    // 遍历结果链表
    struct addrinfo* p = result;
    while (p != nullptr) {
        // 可以访问p->ai_family、p->ai_addr等成员来获取具体的信息
        print_sa_data(p->ai_addr);
        p = p->ai_next;
    }

    // 释放内存
    freeaddrinfo(result);

    return 0;
}