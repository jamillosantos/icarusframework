/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_REQUEST_HPP
#define ICARUSFRAMEWORK_REQUEST_HPP

#include "headers.hpp"

namespace icarus
{
namespace http
{
	class Request
	{
	private:
		bool _headersWritten;
	protected:
		Values _headers;
	public:
		Request()
			: _headersWritten(false)
		{ }

		Values & headers()
		{
			return this->_headers;
		}
	};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_HPP
