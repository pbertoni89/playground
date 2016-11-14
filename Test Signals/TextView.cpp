#include "TextView.hpp"

TextView::TextView(Document& doc) :
	_document(doc)
{
	_connection = _document.connect(boost::bind(&TextView::refresh, this, 3));
}

TextView::~TextView()
{
	_connection.disconnect();
}

void TextView::refresh(int n) const
{
	std::cout << "TextView (" << n << "): " << _document.get_text() << std::endl;
}
