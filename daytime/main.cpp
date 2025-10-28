#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Создание UDP сокета
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Подготовка адресной структуры сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(13); // Порт службы daytime
    server_addr.sin_addr.s_addr = inet_addr("172.16.40.1"); // Адрес тестового сервера

    // Отправка пустого сообщения для запроса времени
    char request[] = "";
    ssize_t send_result = sendto(client_socket, request, strlen(request), 0,
                                (const struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if (send_result == -1) {
        std::cerr << "Ошибка отправки запроса" << std::endl;
        close(client_socket);
        return 1;
    }

    std::cout << "Запрос отправлен на сервер 172.16.40.1:13" << std::endl;

    // Прием ответа от сервера
    char buffer[1024];
    socklen_t server_addr_len = sizeof(server_addr);
    
    // Установка таймаута на прием данных (5 секунд)
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    ssize_t recv_result = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr*)&server_addr, &server_addr_len);
    
    if (recv_result == -1) {
        std::cerr << "Ошибка приема ответа (таймаут или ошибка сети)" << std::endl;
        close(client_socket);
        return 1;
    }

    // Добавление нулевого символа для корректного вывода строки
    buffer[recv_result] = '\0';
    
    // Вывод полученного времени
    std::cout << "Текущее время с сервера: " << buffer;

    // Закрытие сокета
    close(client_socket);
    return 0;
}
