/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** TCP
*/

#ifndef TCP_HPP_
#define TCP_HPP_

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

#define SERVER_PORT 8080
#define CLIENT_PORT 8081


class TCP {

    public:
        enum TYPE { CLIENT, SERVER };

        TCP(TYPE type);
        ~TCP();
        std::string read_socket();
        void send_socket(const std::string& message);
        void disconnect();
        void accept();
    private:
        boost::asio::io_service _io_service;
        tcp::socket _socket { _io_service };
        tcp::acceptor _acceptor { _io_service };
};


#endif /* !TCP_HPP_ */
