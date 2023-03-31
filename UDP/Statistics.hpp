/*
** EPITECH PROJECT, 2020
** Communication
** File description:
** Statistics
*/

#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include <atomic>
#include <ostream>

struct Statistics
{
    public:
        Statistics();
        Statistics(const Statistics& other);
        uint32_t GetReceivedMessages() const;
        uint64_t GetReceivedBytes() const;
        uint32_t GetSentMessages() const;
        uint64_t GetSentBytes() const;
        void RegisterSentMessage(int32_t size);
        void RegisterReceivedMessage(int32_t size);

    private:
        std::atomic_uint32_t receivedMessages;
        std::atomic_uint32_t sentMessages;
        std::atomic_uint64_t receivedBytes;
        std::atomic_uint64_t sentBytes;
};

std::ostream& operator<<(std::ostream& os, const Statistics& obj);

#endif /* !STATISTICS_HPP_ */