#pragma once
#include "CryptoAlgorithm.h"

#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
private:
	std::string key;
	int lastIdxKey;
public:
	OTPCryptoAlgorithm(const std::string& Key) { key = Key; lastIdxKey = 0; };
	virtual ~OTPCryptoAlgorithm() = default;

	virtual Buffer Encrypt(const std::string& msg) override;
	virtual std::string Decrypt(Buffer msg) override;
};