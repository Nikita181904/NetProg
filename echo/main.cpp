#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <сообщение>" << std::endl;
        return 1;
    }

    // Создание TCP сокета
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Подготовка адресной структуры сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7); // Порт службы echo
    server_addr.sin_addr.s_addr = inet_addr("172.16.40.1"); // Адрес тестового сервера

    // Установка соединения с сервером
    std::cout << "Установка соединения с сервером 172.16.40.1:7..." << std::endl;
    int connect_result = connect(client_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if (connect_result == -1) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        close(client_socket);
        return 1;
    }

    std::cout << "Соединение установлено успешно" << std::endl;

    // Отправка сообщения на сервер
    const char* message = argv[1];
    ssize_t send_result = send(client_socket, message, strlen(message), 0);
    
    if (send_result == -1) {
        std::cerr << "Ошибка отправки сообщения" << std::endl;
        close(client_socket);
        return 1;
    }

    std::cout << "Отправлено сообщение: " << message << std::endl;
    std::cout << "Отправлено байт: " << send_result << std::endl;

    // Прием ответа от сервера
    char buffer[1024];
    ssize_t recv_result = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (recv_result == -1) {
        std::cerr << "Ошибка приема ответа" << std::endl;
        close(client_socket);
        return 1;
    }

    // Добавление нулевого символа для корректного вывода строки
    buffer[recv_result] = '\0';
    
    // Вывод полученного эха
    std::cout << "Получено эхо: " << buffer << std::endl;
    std::cout << "Получено байт: " << recv_result << std::endl;

    // Закрытие сокета
    close(client_socket);
    std::cout << "Соединение закрыто" << std::endl;
    
    return 0;
}
