/*
** EPITECH PROJECT, 2020
** Delivery
** File description:
** Log
*/

#ifndef LOG_HPP_
#define LOG_HPP_

#include <iostream>
#include <string>

namespace Log {
    inline void Info(std::string inf)
    {
        std::cout << "INFO: " << inf << std::endl;
    }

    inline void Debug(std::string inf)
    {
        std::cout << "DEBUG: " << inf << std::endl;
    }

    inline void Error(std::string Error) {
        std::cout << "ERROR: " << Error << std::endl;
        exit(81);
    }
    inline void Warning(std::string Error) {
        std::cout << "WARNING: " << Error << std::endl;
        exit(81);
    }
}
#endif /* !LOG_HPP_ */