#pragma once

//using namespace System;

#include "Stdafx.h"
#include "chat_message.hpp"

namespace xspectra
{
	public ref class Message
	{
	public:
		Message();
		static unman_message * message = new unman_message();
	};

}
