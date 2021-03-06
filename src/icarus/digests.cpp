/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 04, 2016
 */

#include <icarus/digests.h>
#include <cstring>
#include <openssl/sha.h>
#include <chrono>

std::string icarus::digests::uniqueid()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return icarus::digests::sha256(
		std::to_string(std::rand())
		+ std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count())
	);
}

void icarus::digests::sha256(const char *string, size_t size, char *buffer)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, size);
	SHA256_Final(hash, &sha256);
	int i = 0;
	for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(buffer + (i * 2), "%02x", hash[i]);
	buffer[64] = 0;
}

std::string icarus::digests::sha256(const std::string &data)
{
	char buffer[65];
	icarus::digests::sha256(data.c_str(), data.length(), buffer);
	return std::string(buffer, 64);
}

