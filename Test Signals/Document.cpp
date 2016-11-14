#include "Document.hpp"

Document::Document()
{
}

Document::~Document()
{
}

boost::signals2::connection Document::connect(const signal_t::slot_type & subscriber)
{
	return _signal.connect(subscriber);
}

void Document::append(const char * s)
{
	_sText += s;
	_signal();
}

const std::string& Document::get_text() const
{
	return _sText;
}
