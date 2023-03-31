/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** IClient
*/

#include <string>

class IClient {
    public:
        virtual ~IClient() {};
        virtual bool HasMessages() = 0;
        virtual void Send(const std::string& message) = 0;
        virtual std::string PopMessage() = 0;
};
