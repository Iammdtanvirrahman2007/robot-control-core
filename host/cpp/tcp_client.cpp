#include "tcp_client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

TcpClient::TcpClient() : socket_fd_(-1) {}

TcpClient::~TcpClient() {
    disconnect();
}

bool TcpClient::connect_to(const std::string& host, uint16_t port) {
    disconnect();

    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        return false;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1) {
        disconnect();
        return false;
    }

    if (connect(socket_fd_,
                reinterpret_cast<sockaddr*>(&address),
                sizeof(address)) < 0) {
        disconnect();
        return false;
    }

    return true;
}

bool TcpClient::send_line(const std::string& line) {
    if (!connected()) {
        return false;
    }

    std::string message = line;
    if (message.empty() || message.back() != '\n') {
        message += '\n';
    }

    const char* data = message.data();
    std::size_t remaining = message.size();

    while (remaining > 0) {
        const ssize_t sent = send(socket_fd_, data, remaining, 0);
        if (sent <= 0) {
            disconnect();
            return false;
        }

        data += sent;
        remaining -= static_cast<std::size_t>(sent);
    }

    return true;
}

void TcpClient::disconnect() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
        socket_fd_ = -1;
    }
}

bool TcpClient::connected() const {
    return socket_fd_ >= 0;
}
