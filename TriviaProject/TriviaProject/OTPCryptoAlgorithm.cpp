#include "OTPCryptoAlgorithm.h"

Buffer OTPCryptoAlgorithm::Encrypt(const std::string& msg)
{
	Buffer result;
	for (int i = 0; i < msg.size(); i++)
	{
		int sum = (int)msg[i] + (int)key[i % key.size()];
		if (sum > 255)
			sum -= 255;
		result.buffer.push_back((unsigned char)sum);
	}
	return result;
}

std::string OTPCryptoAlgorithm::Decrypt(Buffer msg)
{
	std::string result = "";
	for (int i = 0; i < msg.buffer.size(); i++)
	{
		int dif = (int)msg.buffer[i] - (int)key[i % key.size()];
		if (dif < 0)
			dif += 255;
		result.push_back((char)dif);
	}
	return result;
}
