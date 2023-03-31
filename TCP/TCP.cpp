/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** TCP
*/

#include "TCP.hpp"

TCP::TCP(TYPE type) {
    if (type == CLIENT) {
        _socket.connect(tcp::endpoint{ {}, SERVER_PORT});
    } else {
        _acceptor = tcp::acceptor{_io_service, {{}, SERVER_PORT}};
    }
}

TCP::~TCP() {
    if (_socket.is_open())
        _socket.close();
    if (_acceptor.is_open()) _acceptor.close();
}

std::string TCP::read_socket() {
    std::string data;
    auto n = read_until(_socket, boost::asio::dynamic_buffer(data), "\n");
    data.resize(n);
    return data;
}

void TCP::send_socket(const std::string& message) {
    write(_socket, boost::asio::buffer(message + "\n"));
}

void TCP::disconnect() {
    _socket.close();
}

void TCP::accept()
{
    _acceptor.accept(_socket);
}