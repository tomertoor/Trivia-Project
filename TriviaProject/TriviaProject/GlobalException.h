#pragma once
#include <exception>
#include <string>

class GlobalException : public std::exception
{
public:
	GlobalException(const std::string& message) : _message(message) {}
	virtual ~GlobalException() noexcept = default;

	virtual const char* what() const noexcept { return _message.c_str(); }

protected:
	std::string _message;

};