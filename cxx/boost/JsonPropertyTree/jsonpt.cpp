/* 
 * http://zenol.fr/blog/boost-property-tree/en.html
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "boost/multi_array.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>


// Short alias for this namespace
namespace pt = boost::property_tree;



/*
 * 
 */
int main(int argc, char** argv)
{
	int * r;
	std::cout << "sizeof(int*) = " << sizeof(r) << std::endl;
	// Create a root
	pt::ptree root;

	// Load the json file in this ptree
	pt::read_json("foo.json", root);

	// parsing a 2D matrix from a list of lists
	int matrix[3][3];
	int i = 0, j;

	pt::ptree mesh = root.get_child("meshGenerators");
	for (pt::ptree::value_type & row : mesh.get_child("data"))
	{
		j = 0;
		for (pt::ptree::value_type & cell : row.second)
		{
			const auto VAL = cell.second.get_value<double>();
			matrix[i][j] = VAL;
			std::cout << "\tm[" << i << ", " << j << "] = " << VAL;
			j++;
		}
		std::cout << std::endl;
		i++;
	}

	// Read values
	int height = root.get<int>("height", 0);

	// You can also go through nested nodes
	std::string msg = root.get<std::string>("some.complex.path");

	// A vector to allow storing our animals
	std::vector<std::pair<std::string, std::string>> animals;

	// Iterator over all animals
	for (const auto & animal : root.get_child("animals"))
	{
		// animal is a pt::ptree::value_type, thus a std::pair of a string and a child

		// Get the label of the node
		std::string name = animal.first;
		// Get the content of the node
		std::string color = animal.second.data();
		animals.push_back(std::make_pair(name, color));
		std::cout << "animal " << name << ", " << color << std::endl;
	}


	std::vector<std::string> fruits;
	for (const pt::ptree::value_type & fruit : root.get_child("fruits"))
	{
		// fruit.first contain the string ""
		fruits.push_back(fruit.second.data());
	}
	
	return 0;
}
