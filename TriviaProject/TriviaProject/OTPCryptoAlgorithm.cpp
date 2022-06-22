#include "OTPCryptoAlgorithm.h"

Buffer OTPCryptoAlgorithm::Encrypt(const std::string& msg)
{
	Buffer result;
	lastIdxKey = 0;
	for (int i = 0; i < 16; i++)
		key.push_back(CHARS[rand() % 36]);
	for (int i = 0; i < msg.size(); i++)
	{
		int sum = ((int)msg[i] + (int)key[i % key.size()]) % 255;
		result.buffer.push_back((unsigned char)sum);
	}
	return result;
}

std::string OTPCryptoAlgorithm::Decrypt(Buffer msg)
{
	std::string result = "";
	for (int i = 0;  i < msg.buffer.size(); lastIdxKey++, i++)
	{
		int dif = ((int)msg.buffer[i] - (int)key[lastIdxKey % key.size()]) % 255;
		result.push_back((char)dif);
	}
	return result;
}

std::string OTPCryptoAlgorithm::GetKey()
{
	return key;
}
