#pragma once

#include "Document.hpp"
#include <iostream>

class TextView
{
	Document & _document;

	boost::signals2::connection _connection;

public:

	TextView(Document & doc);

	~TextView();

	void refresh(int n) const;
};
