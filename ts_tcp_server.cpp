#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 结构体类型
enum MessageType {
    PERSON,
    ANIMAL,
    CAR
};

// Person 结构体
struct Person {
    char name[20];
    int age;
};

// Animal 结构体
struct Animal {
    char species[20];
    float weight;
};

// Car 结构体
struct Car {
    char brand[20];
    int year;
};

void handleClient(int clientSocket) {
    char buffer[1024];
    int bytesRead;

    while (true) {
        MessageType type;
        memset(&type, 0, sizeof(type));
        bytesRead = recv(clientSocket, &type, sizeof(type), 0);

        if (bytesRead <= 0) {
            break;
        }

        if (type == PERSON) {
            Person person;
            memset(&person, 0, sizeof(person));
            bytesRead = recv(clientSocket, &person, sizeof(person), 0);

            if (bytesRead <= 0) {
                break;
            }

            std::cout << "Received Person: " << person.name << ", " << person.age << " years old" << std::endl;
        }
        else if (type == ANIMAL) {
            Animal animal;
            memset(&animal, 0, sizeof(animal));
            bytesRead = recv(clientSocket, &animal, sizeof(animal), 0);

            if (bytesRead <= 0) {
                break;
            }

            std::cout << "Received Animal: " << animal.species << ", " << animal.weight << "kg" << std::endl;
        }
        else if (type == CAR) {
            Car car;
            memset(&car, 0, sizeof(car));
            bytesRead = recv(clientSocket, &car, sizeof(car), 0);

            if (bytesRead <= 0) {
                break;
            }

            std::cout << "Received Car: " << car.brand << ", " << car.year << std::endl;
        }
    }

    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;

    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8888);

    // 绑定套接字到服务器地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    // 监听连接请求
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return -1;
    }

    std::cout << "Server started, waiting for connections..." << std::endl;

    // 接受连接请求并处理客户端
    while (true) {
        clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::cout << "Connected with client: " << clientIP << std::endl;

        handleClient(clientSocket);
    }

    // 关闭服务器套接字
    close(serverSocket);

    return 0;
}
