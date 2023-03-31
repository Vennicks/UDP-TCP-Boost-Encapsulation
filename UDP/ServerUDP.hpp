/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** ServerUDP
*/

#ifndef ServerUDP_HPP_
#define ServerUDP_HPP_

#include "Constants.hpp"
#include "Statistics.hpp"

#include "LockedQueue.hpp"

#include <boost/asio.hpp>
#include "Log.hpp"
#include <array>
#include <map>
#include <thread>
#include <atomic>
#include "IServer.hpp"

using boost::asio::ip::udp;

typedef std::map<uint32_t, udp::endpoint> ClientList;
typedef ClientList::value_type Client;

class ServerUDP : public IServer {
    public:
        explicit ServerUDP(unsigned short local_port);
        virtual ~ServerUDP();
        bool HasMessages() override;
        ClientMessage PopMessage() override;
        void SendToClient(const std::string& message, uint32_t clientID) override;
        void SendToAllExcept(const std::string& message, uint32_t clientID);
        void SendToAll(const std::string& message);
        size_t GetClientCount() override;
        uint32_t GetClientIdByIndex(size_t index) override;
        const Statistics& GetStatistics() const { return statistics; };
        std::vector<std::function<void(uint32_t)>> clientDisconnectedHandlers;

    private:
        // Network send/receive stuff
        boost::asio::io_service io_service;
        udp::socket socket;
        udp::endpoint ServerUDP_endpoint;
        udp::endpoint remote_endpoint;
        std::array<char, NetworkBufferSize> recv_buffer;
        std::thread service_thread;

        // Low-level network functions
        void start_receive();
        void handle_remote_error(const std::error_code error_code, const udp::endpoint remote_endpoint);
        void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
        void handle_send(std::string /*message*/, const std::error_code& /*error*/, std::size_t /*bytes_transferred*/)	{}
        void run_service();

        // Client management
        int32_t get_or_create_client_id(udp::endpoint endpoint);
        void on_client_disconnected(int32_t id);

        void send(const std::string& message, udp::endpoint target);

        // Incoming messages queue
        LockedQueue<ClientMessage> incomingMessages;

        // Clients of the ServerUDP
        ClientList clients;
        std::atomic_int32_t nextClientID;

        ServerUDP(ServerUDP&); // block default copy constructor

        // Statistics
        Statistics statistics;
};
#endif /* !ServerUDP_HPP_ */