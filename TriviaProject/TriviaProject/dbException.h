#include <iostream>

#define USER_EXIST 1
#define WRONG_PASS 2
#define ALREADY_LOGGED 3

class dbException
{
public:
    int status;
    dbException(int status) : status(status) {};
    ~dbException() {};
};