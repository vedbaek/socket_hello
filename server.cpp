#include "message.h"
#include <iostream> // cout, cerr
#include <sys/socket.h> // socket()、bind() 和 connect()
#include <arpa/inet.h> // inet_pton() 和 inet_ntop() 函数，用于进行 IP 地址的二进制和字符串的转换
#include <unistd.h> // close()，用于关闭文件描述符，包括套接字
#include <string>

using namespace std;

int main() {
	// 1. 创建socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Failed to create socket" << endl;
		return 1;
	}
	cout << "Socket created!" << endl;

	// 2. 设置socket地址
	sockaddr_in server_addr{}, client_addr{};
	int server_port = 8080;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(server_port);

	// 3. 绑定socket
	if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Failed to bind socket" << endl;
		close(server_socket);
		return 1;
	}
	cout << "Socket binded!" << endl;

	// 4. 开始监听
	if (listen(server_socket, SOMAXCONN) == -1) {
		std::cerr << "Failed to listen on socket" << endl;
		return 1;
	}
	cout << "Listenning..." << endl;

	// 5. 等待连接
	socklen_t client_len = sizeof(client_addr);
	int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
	if (client_socket == -1) {
		std::cerr << "Failed to accept connection" << endl;
		return 1;
	}
	cout << "Client accepted!" << endl;

	// 6. 从客户端接收数据
	char buff[1024]{};
	int len = recv(client_socket, buff, sizeof(buff), 0);
	if (len == -1) {
		std::cerr << "Failed to receive data from client" << endl;
		close(client_socket);
		close(server_socket);
		return 1;
	}
	cout << "Message from client: " << buff << endl;

	// 7. 发送响应给客户端
	string msg = "Hello from server";
	if (send(client_socket, msg.c_str(), msg.length(), 0) == -1) {
		std::cerr << "Failed to send data to client" << endl;
		close(client_socket);
		close(server_socket);
		return 1;
	}
	cout << "Message sent to client" << endl;

	// 8. 关闭socket
	close(client_socket);
	close(server_socket);

	return 0;
}
