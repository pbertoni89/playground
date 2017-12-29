#pragma once

#include <boost/signals2.hpp>

typedef boost::signals2::signal<void()> signal_t;

class Document
{
	signal_t _signal;

	std::string _sText;

public:

	Document();

	~Document();

	/* Connect a slot to the signal which will be emitted whenever text is appended to the document. */
	boost::signals2::connection connect(const signal_t::slot_type & subscriber);

	void append(const char * s);

	const std::string& get_text() const;
};
