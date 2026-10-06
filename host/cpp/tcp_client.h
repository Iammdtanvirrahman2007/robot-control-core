#pragma once

#include <cstdint>
#include <string>

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connect_to(const std::string& host, uint16_t port);
    bool send_line(const std::string& line);
    void disconnect();
    bool connected() const;

private:
    int socket_fd_;
};
