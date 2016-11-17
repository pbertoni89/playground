#include "stdafx.h"

#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#include <iostream>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Linq;
using namespace System::Xml::Linq;

void foo_parse()
{
	XDocument^ xd = XDocument::Load("XmlFile.xml");

	XName^ xnPbElement("pbElement");
	XName^ xnPbAttribute("pbAttribute");

	// Put the elements in a List of XElements (easier to navigate)
	List<XElement^>^ xeList = Enumerable::ToList(xd->Element("root")->Elements(xnPbElement));

	// Write the individual attributes and elements
	Console::WriteLine(xeList[0]->Attribute(xnPbAttribute)->Value + "=" + xeList[0]->Value);
	Console::WriteLine(xeList[1]->Attribute(xnPbAttribute)->Value + "=" + xeList[1]->Value);
	Console::WriteLine(xeList[2]->Attribute(xnPbAttribute)->Value + "=" + xeList[2]->Value);

	// Loop through each attribute and element
	for each (XElement^ xe in xeList)
	{
		Console::WriteLine(xe->Attribute(xnPbAttribute)->Value + "=" + xe->Value);
	}
}

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Managed Hello World");
	std::cout << "Unmanaged Hello World" << std::endl << std::endl;

	foo_parse();

	system("PAUSE");
    return 0;
}
