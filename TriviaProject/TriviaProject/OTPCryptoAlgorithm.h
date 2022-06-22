#pragma once
#include "CryptoAlgorithm.h"

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
private:
	std::string key;
public:
	OTPCryptoAlgorithm() { key = "hHJufds89u2ik90fds23jky8sthj"; };
	virtual ~OTPCryptoAlgorithm() = default;

	virtual Buffer Encrypt(const std::string& msg) override;
	virtual std::string Decrypt(Buffer msg) override;
};