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
#include "debug_output.hpp"

/**
* Debug/Tracefile generation
*
* Two types of debug/trace files are available with Connector/C++. You can make
* Connector/C++ write a trace file and/or activate tracing on the level of the
* underlying MySQL Client Libarary.
*
* Connector/C++ is based on the MySQL Client Library (C-API, AKA libmysql).
* Connector/C++ is a proxy/wrapper of the MySQL Client Library.
* Debug versions of the MySQL Client Library can write a trace file. On the C-level
* this is done by passing a std::string with debug instructions to the C function
* mysql_debug(). You can use the Connector/C++ method
* setClientOption("libmysql_debug", const std::string debug) to pass a std::string to the
* C-API function mysql_debug(). See the MySQL C-API documentation
*
* Example of a libmysql trace:
* >mysql_stmt_init
* | >_mymalloc
* | | enter: Size: 816
* | | exit: ptr: 0x68e7b8
* | <_mymalloc
* | >init_alloc_root
* | | enter: root: 0x68e7b8
* | | >_mymalloc
* | | | enter: Size: 2064
* | | | exit: ptr: 0x68eb28
* [...]
*
* If you do not have compiled Connector/C++ against a debug version of the MySQL
* Client Library but you have compiled  Connector/C++ with tracing enabled, then
* you may activate the Connector/C++ internal trace using
* setClientOption("clientTrace", bool on_off).
*
* To compile Connector/C++ with tracing enabled, do:
*   cmake -DMYSQLCPPCONN_TRACE_ENABLE:BOOL=1
*
* Example of a Connector/C++ trace:
* |  INF: Tracing enabled
* <MySQL_Connection::setClientOption
* >MySQL_Prepared_Statement::setInt
* |  INF: this=0x69a2e0
* |  >MySQL_Prepared_Statement::checkClosed
* |  <MySQL_Prepared_Statement::checkClosed
* | <MySQL_Prepared_Statement::setInt
*
* Note that the traces will contain your SQL statements. You may not want
* to share sensitive SQL statements.
*
* @link http://dev.mysql.com/doc/refman/5.1/en/mysql-debug.html
*/


int main_debug_output(int argc, const char **argv)
{
	const std::string host(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	sql::Driver *driver;
	int i;
	int on_off = 1;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;;
	std::cout << "# Connector/C++ debug / client_options example.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(host, user, pass));

		/*
		 Activate debug trace of the MySQL Client Library (C-API)
		Only available with a debug build of the MySQL Client Library!
		*/
		con->setClientOption("libmysql_debug", "d:t:O,client.trace");

		con->setSchema(database);

		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, label CHAR(1))");
		std::cout << "#\t Test table created" << std::endl;

		boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("INSERT INTO test(id, label) VALUES (?, ?)"));
		for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			prep_stmt->setInt(1, test_data[i].id);
			prep_stmt->setString(2, test_data[i].label);
			prep_stmt->executeUpdate();

			/*
			Pointless activity to demo how to
			activating and de-activating the Connector/C++ internal trace
			*/
			on_off = (i + 2) % 2;
			con->setClientOption("clientTrace", &on_off);
		}

		stmt->execute("DROP TABLE IF EXISTS test");

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		std::cout << std::endl;
		std::cout << "# ERR: DbcException in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		// Use what(), getErrorCode() and getSQLState()
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		if (e.getErrorCode() == 1047) {
			/*
			Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
			Message: Unknown command
			*/
			std::cout << "# ERR: Your server seems not to support PS at all because its MYSQL <4.1" << std::endl;
		}
		std::cout << "not ok 1 - examples/debug.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << std::endl;
		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/debug.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/debug.cpp" << std::endl;
	return EXIT_SUCCESS;
}
