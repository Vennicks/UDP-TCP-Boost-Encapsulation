/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** LockedQueue
*/

#ifndef LOCKEDQUEUE_HPP_
#define LOCKEDQUEUE_HPP_
#include <mutex>
#include <queue>
#include <list>

template<typename _T> class LockedQueue {
    public:
        void push(_T value) {
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(value);
        };

        // Get top message in the queue
        // Note: not exception-safe (will lose the message)
        _T pop() {
            std::unique_lock<std::mutex> lock(mutex);
            _T value;
            std::swap(value, queue.front());
            queue.pop();
            return value;
        };

        bool empty() {
            std::unique_lock<std::mutex> lock(mutex);
            return queue.empty();
        }

        int size() {
            std::unique_lock<std::mutex> lock(mutex);
            return queue.size();
        }
    private:
        std::mutex mutex;
        std::queue<_T> queue;
};

#endif /* !LOCKEDQUEUE_HPP__HPP_ */