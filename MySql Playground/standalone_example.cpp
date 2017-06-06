/*
Copyright (c) 2008, 2014, Oracle and/or its affiliates. All rights reserved.

The MySQL Connector/C++ is licensed under the terms of the GPLv2
<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
MySQL Connectors. There are special exceptions to the terms and
conditions of the GPLv2 as it is applied to this software, see the
FLOSS License Exception
<http://www.mysql.com/about/legal/licensing/foss-exception.html>.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "stdafx.h"
#include "standalone_example.hpp"

/**
* Basic example of creating a stand alone program linked against Connector/C++
*
* This example is not integrated into the Connector/C++ build environment.
* You must run "make install" prior to following the build instructions
* given here.
*
* To compile the standalone example on Linux try something like:
*
* /usr/bin/c++
*   -o standalone
*   -I/usr/local/include/cppconn/
*   -Wl,-Bdynamic -lmysqlcppconn
*    examples/standalone_example.cpp
*
* To run the example on Linux try something similar to:
*
*  LD_LIBRARY_PATH=/usr/local/lib/ ./standalone
*
* or:
*
*  LD_LIBRARY_PATH=/usr/local/lib/ ./standalone host user password database
*
*/


/**
* Usage example for Driver, Connection, (simple) Statement, ResultSet
*/
int main_standalone_example(int argc, const char **argv)
{
	std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	std::cout << std::endl;
	std::cout << "Connector/C++ standalone program example..." << std::endl;
	std::cout << std::endl;

	try {
		sql::Driver * driver = sql::mysql::get_driver_instance();
		/* Using the Driver to create a connection */
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));
		con->setSchema(database);

		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));
		std::cout << "\t... running 'SELECT 'Welcome to Connector/C++' AS _message'" << std::endl;
		while (res->next()) {
			std::cout << "\t... MySQL replies: " << res->getString("_message") << std::endl;
			std::cout << "\t... say it again, MySQL" << std::endl;
			std::cout << "\t....MySQL replies: " << res->getString(1) << std::endl;
		}

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << std::endl;
	std::cout << "... find more at http://www.mysql.com" << std::endl;
	std::cout << std::endl;
	return EXIT_SUCCESS;
}
