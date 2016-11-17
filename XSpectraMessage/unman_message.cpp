#include "unman_message.hpp"

namespace xspectra
{
	unman_message::unman_message() :
		_body_length(0)
	{
	}

	unman_message::unman_message(const char * sBody)
	{
		set_body_length(std::strlen(sBody));
		std::memcpy(body(), sBody, _body_length);
		encode_header();
	}

	const char *
	unman_message::data()
	const
	{
		return _data;
	}

	char *
	unman_message::data()
	{
		return _data;
	}

	std::size_t
	unman_message::length()
	const
	{
		return header_length + _body_length;
	}

	const char *
	unman_message::body()
	const
	{
		return _data + header_length;
	}

	char *
	unman_message::body()
	{
		return _data + header_length;
	}

	const std::string
	unman_message::to_string()
	const
	{
		return std::string(body(), _body_length);
	}

	std::size_t
	unman_message::get_body_length()
	const
	{
		return _body_length;
	}

	void
	unman_message::set_body_length(std::size_t new_length)
	{
		_body_length = new_length;

		if (_body_length > max_body_length)
			_body_length = max_body_length;
	}

	bool
	unman_message::decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, _data, header_length);
		_body_length = std::atoi(header);
		if (_body_length > max_body_length)
		{
			_body_length = 0;
			return false;
		}
		return true;
	}

	void
	unman_message::encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(_body_length));
		std::memcpy(_data, header, header_length);
	}
}
