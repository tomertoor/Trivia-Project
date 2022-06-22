#pragma once
#include "CryptoAlgorithm.h"

#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

class PAZCryptoAlgorithm : public CryptoAlgorithm
{
private:
	std::string key;
	int lastIdxKey;
public:
	PAZCryptoAlgorithm(const std::string& Key) { key = Key; lastIdxKey = 0; };
	virtual ~PAZCryptoAlgorithm() = default;

	virtual Buffer Encrypt(const std::string& msg) override;
	virtual std::string Decrypt(Buffer msg) override;
	std::string GetKey();
};