#pragma once

#include <iostream>

#define NO_QUESTIONS 0

class GameException : public std::exception
{
public:
    unsigned int status;
    GameException(int status) : status(status) {};
    ~GameException() {};
};