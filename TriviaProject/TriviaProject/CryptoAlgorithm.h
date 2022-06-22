#pragma once
#include <string>
#include "Buffer.h"

class CryptoAlgorithm
{
public:
	CryptoAlgorithm()= default;
	virtual ~CryptoAlgorithm() = default;

	virtual Buffer Encrypt(const std::string& msg) = 0;
	virtual std::string Decrypt(Buffer msg) = 0;
};