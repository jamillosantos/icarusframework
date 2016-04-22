/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 04, 2016
 */

#ifndef ICARUS_DIGESTS_H
#define ICARUS_DIGESTS_H

#include <string>

namespace icarus
{
namespace digests
{
std::string uniqueid();

void sha256(const char *string, size_t size, char *buffer);
std::string sha256(const std::string &data);
}
}

#endif //ICARUS_DIGESTS_H
