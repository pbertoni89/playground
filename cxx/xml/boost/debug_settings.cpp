// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

//[debug_settings_includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
namespace pt = boost::property_tree;


//]
//[debug_settings_data
struct debug_settings
{
    std::string m_file;               // log filename
    int m_level;                      // debug level
    std::set<std::string> m_modules;  // modules where logging is enabled
    void load(const std::string &filename);
    void save(const std::string &filename);
};
//]
//[debug_settings_load


void debug_settings::load(const std::string &filename)
{
    pt::ptree tree;
    pt::read_xml(filename, tree);
    m_file = tree.get<std::string>("debug.filename");
    m_level = tree.get("debug.level", 0);

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("debug.modules")) {
        // The data function is used to access the data stored in a node.
        m_modules.insert(v.second.data());
		std::cout << "reading " << v.second.data() << '\n';
		std::cout << "reading attr " << v.second.data() << '\n';
		std::cout << "reading node " << v.second.get<std::string>("") << '\n';
    }
    std::cout << "\n\ndebug filename " << tree.get<std::string>("debug.filename") << '\n';
	std::cout << "\n\nx " << tree.get<std::string>("modules.Finance") << '\n';
}


void debug_settings::save(const std::string &filename)
{
    // Create an empty property tree object.
    pt::ptree tree;

    // Put the simple values into the tree. The integer is automatically
    // converted to a string. Note that the "debug" node is automatically
    // created if it doesn't exist.
    tree.put("debug.filename", m_file);
    tree.put("debug.level", m_level);

    // Add all the modules. Unlike put, which overwrites existing nodes, add
    // adds a new node at the lowest level, so the "modules" node will have
    // multiple "module" children.
    BOOST_FOREACH(const std::string &name, m_modules)
	{
		std::cout << "saving " << name << '\n';
        tree.add("debug.modules.module", name);
	}

    // Write property tree to XML file
    pt::write_xml(filename, tree);
}
//]


int main()
{
    try
    {
        debug_settings ds;
        ds.load("debug_settings.xml");
        ds.save("debug_settings_out.xml");
        std::cout << "Success\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
