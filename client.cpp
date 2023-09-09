#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main() {
	// 1. 创建socket
	// Protocal Family: AF_INET使用IPv4协议族，可选AF_INET6，即IPv6，和AF_UNIX本地通信
	// Socket Type: SOCK_STREAM TCP流式socket，可选SOCK_DGRAM, UDP数据报，SOCK_RAW，可编辑IP报文
	// Protocal: 0表示使用默认协议
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		cout << "Failed to create socket" << endl;
		return 1;
	}

	// 2. 设置socket地址
	sockaddr_in server_addr{};
	int server_port = 8080;
	std::string server_ip = "127.0.0.1";
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	server_addr.sin_port = htons(server_port);

	// 3. 连接服务器
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr) == -1)) {
		std::cerr << "Failed to connect to server" << endl;
		return 1;
	}

	// 4. 发送数据
	string msg = "Hello from client!";
	if (send(client_socket, msg.c_str(), msg.length(), 0) == -1) {
		std::cerr << "Failed to send data: " << msg << endl;
		return 1;
	}

	// 5. 等待响应
	char buff[1024]{};
	int len = recv(client_socket, buff, sizeof(buff), 0);
	if (len == -1) {
		std::cerr << "Failed to receive data" << endl;
		close(client_socket);
		return 1;
	}
	cout << "Message from server: " << buff << endl;

	// 6. 关闭socket
	close(client_socket);
	return 0;
}

