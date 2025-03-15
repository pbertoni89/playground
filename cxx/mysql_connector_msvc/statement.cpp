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
#include "statement.hpp"

/* *
* Example of sql::Statement - "simple" (not prepared) statements
*
*/


/**
* Example of statements - not to be confused with prepared statements
*/
int main_statement(int argc, const char **argv)
{
	static const std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	static const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	static const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	static const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	/* Driver Manager */
	sql::Driver *driver;

	/* sql::ResultSet.rowsCount() returns size_t */
	size_t row;
	std::stringstream sql;
	int i;
	bool ok;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ (simple) statement example.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));
		con->setSchema(database);

		/* Creating a "simple" statement - "simple" = not a prepared statement */
		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

		/* Create a test table demonstrating the use of sql::Statement.execute() */
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, label CHAR(1))");
		std::cout << "#\t Test table created" << std::endl;

		/* Populate the test table with data */
		for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			// KLUDGE: You should take measures against SQL injections!
			// example.h contains the test data
			sql.str("");
			sql << "INSERT INTO test(id, label) VALUES (";
			sql << test_data[i].id << ", '" << test_data[i].label << "')";
			stmt->execute(sql.str());
		}
		std::cout << "#\t Test table populated" << std::endl;

		/*
		NOTE: Use execute() instead of the more convenient executeQuery()
		See the other example file for executeQuery() and executeUpdate() examples
		However, if you are executing SQL dynamically, you might have to use execute()
		*/
		ok = stmt->execute("SELECT id, label FROM test ORDER BY id ASC");
		std::cout << "#\t stmt->execute('SELECT id, label FROM test ORDER BY id ASC') = ";
		std::cout << ok << std::endl;
		if (ok == true) {
			/* The first result is a result set */
			std::cout << "#\t\t Fetching results" << std::endl;
			/*
			NOTE: If stmt.getMoreResults() would be implemented already one
			would use a do { ... } while (stmt.getMoreResults()) loop
			*/
			boost::scoped_ptr< sql::ResultSet > res(stmt->getResultSet());
			row = 0;
			while (res->next()) {
				std::cout << "#\t\t Row " << row << " - id = " << res->getInt("id");
				std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
				row++;
			}

		} else if (ok == false) {
			/* The first result is an update count */
			throw std::runtime_error("Expecting regular result set");
		}

		/* Clean up */
		stmt->execute("DROP TABLE IF EXISTS test");
		std::cout << "# done!" << std::endl;

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		// Use what(), getErrorCode() and getSQLState()
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "not ok 1 - examples/statement.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/statement.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/statement.cpp" << std::endl;
	return EXIT_SUCCESS;
}
