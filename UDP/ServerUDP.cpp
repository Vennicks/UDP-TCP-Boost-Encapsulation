/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** ServerUDP
*/

#include "ServerUDP.hpp"

ServerUDP::ServerUDP(unsigned short local_port) :
    socket(io_service, udp::endpoint(udp::v4(), local_port)),
    service_thread(&ServerUDP::run_service, this),
    nextClientID(0L)
{
    std::string info = "Starting ServerUDP on port " + local_port;
    Log::Info(info);
}

ServerUDP::~ServerUDP()
{
    io_service.stop();
    service_thread.join();
}

void ServerUDP::start_receive()
{
    socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
        [this](std::error_code ec, std::size_t bytes_recvd){ this->handle_receive(ec, bytes_recvd); });
}

void ServerUDP::on_client_disconnected(int32_t id)
{
    for (auto& handler : clientDisconnectedHandlers)
        if (handler)
            handler(id);
}

void ServerUDP::handle_remote_error(const std::error_code error_code, const udp::endpoint remote_endpoint)
{
    bool found = false;
    int32_t id;
    for (const auto& client : clients)
        if (client.second == remote_endpoint) {
            found = true;
            id = client.first;
            break;
        }
    if (found == false)
        return;
    clients.erase(id);
    on_client_disconnected(id);
}

void ServerUDP::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        try {
            auto message = ClientMessage(std::string(recv_buffer.data(), recv_buffer.data() + bytes_transferred), get_or_create_client_id(remote_endpoint));
            if (!message.first.empty())
                incomingMessages.push(message);
            statistics.RegisterReceivedMessage(bytes_transferred);
        }
        catch (std::exception ex) {
            std::string Error = "handle_receive: Error parsing incoming message:" + (std::string) ex.what();
            Log::Error(Error);
        } catch (...) {
            Log::Error("handle_receive: Unknown error while parsing incoming message");
        }
    }
    else
    {
        std::string Error = "handle_receive: error: " + error.message() + " while receiving from address";
        Log::Error(Error);
        handle_remote_error(error, remote_endpoint);
    }
    start_receive();
}

void ServerUDP::send(const std::string& message, udp::endpoint target_endpoint)
{
    socket.send_to(boost::asio::buffer(message), target_endpoint);
    statistics.RegisterSentMessage(message.size());
}

void ServerUDP::run_service()
{
    start_receive();
    while (!io_service.stopped()){
        try {
            io_service.run();
        }
        catch (const std::exception& e) {
            std::string Error = "ServerUDP: Network exception: " + (std::string)e.what();
            Log::Error(Error);
        } catch (...) {
            Log::Error("ServerUDP: Network exception: unknown");
        }
    }
    Log::Debug("ServerUDP network thread stopped");
};

int32_t ServerUDP::get_or_create_client_id(udp::endpoint endpoint)
{
    for (const auto& client : clients)
        if (client.second == endpoint)
            return client.first;
    auto id = ++nextClientID;
    clients.insert(Client(id, endpoint));
    return id;
};

void ServerUDP::SendToClient(const std::string& message, uint32_t clientID)
{
    try {
        send(message, clients.at(clientID));
    } catch (std::out_of_range) {
        std::string Error = "Unknown client ID " + std::to_string(clientID);
        Log::Error(Error);
    }
};

void ServerUDP::SendToAllExcept(const std::string& message, uint32_t clientID)
{
    for (auto client : clients)
        if (client.first != clientID)
            send(message, client.second);
};

void ServerUDP::SendToAll(const std::string& message)
{
    for (auto client : clients)
        send(message, client.second);
}

size_t ServerUDP::GetClientCount()
{
    return clients.size();
}

uint32_t ServerUDP::GetClientIdByIndex(size_t index)
{
    auto it = clients.begin();
    for (int i = 0; i < index; i++)
        ++it;
    return it->first;
};

ClientMessage ServerUDP::PopMessage() {
    return incomingMessages.pop();
}

bool ServerUDP::HasMessages()
{
    return !incomingMessages.empty();
}