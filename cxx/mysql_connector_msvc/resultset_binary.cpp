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
#include "resultset_binary.hpp"

/* *
* Handling binary data using getString()
*
*/


int main_resultset_binary(int argc, const char **argv)
{
	static const std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	static const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	static const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	static const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	/* Driver Manager */
	sql::Driver *driver;

	/* sql::ResultSet.rowsCount() returns size_t */
	std::stringstream sql;
	int i;
	struct _test_data min, max;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ result set.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));

		con->setSchema(database);

		/* Creating a "simple" statement - "simple" = not a prepared statement */
		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(id INT, label CHAR(1), col_binary BINARY(4), col_varbinary VARBINARY(10))");
		std::cout << "#\t Test table created" << std::endl;

		/* Populate the test table with data */
		min = max = test_data[0];
		for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			/* Remember mnin/max values for further testing */
			if (test_data[i].id < min.id) {
				min = test_data[i];
			}
			if (test_data[i].id > max.id) {
				max = test_data[i];
			}

			/*
			KLUDGE: You should take measures against SQL injections!
			*/
			sql.str("");
			sql << "INSERT INTO test(id, label, col_binary, col_varbinary) VALUES (";
			sql << test_data[i].id << ", '" << test_data[i].label << "', ";
			sql << "\"a\\0b\", '" << i * 5 << "\\0abc')";
			stmt->execute(sql.str());
		}
		std::cout << "#\t Test table populated" << std::endl;

		std::cout << "#\t Testing sql::Statement based resultset" << std::endl;
		{
			boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT * FROM test ORDER BY id ASC"));
			int row = 0;
			while (res->next()) {
				std::cout << "#\t\t Row " << row << ", getRow() " << res->getRow();
				std::cout << " id = " << res->getInt("id");
				std::cout << ", label = '" << res->getString("label") << "'";
				/* std::cout might "hide" \0 and other special characters from the output */
				std::cout << ", col_binary = '" << res->getString("col_binary") << "'";
				std::cout << ", col_varbinary = '" << res->getString("col_varbinary") << "'" << std::endl;

				/* fixed length column - length = size of the column! */
				if (res->getString("col_binary").length() != 4) {
					throw std::runtime_error("BINARY(n) should return std::string of length n regardless how long the value stored in the column is.");
				}

				if (res->getString("col_binary").compare(0, 1, "a")) {
					throw std::runtime_error("First sign from BINARY(n) seems wrong");
				}

				if (res->getString("col_binary").compare(2, 1, "b")) {
					throw std::runtime_error("Third sign from BINARY(n) seems wrong");
				}

				if (res->getString("col_varbinary").length() != 5 && res->getString("col_varbinary").length() != 6) {
					throw std::runtime_error("VARBINARY(n) should return std::string of length n which holds the length of the actual column value.");
				}
				sql::ResultSetMetaData * meta = res->getMetaData();
				std::cout << "#\t\t COLUMN_SIZE = " << meta->getColumnDisplaySize(3) << std::endl;
				row++;
			}
		}

		/* Clean up */
		/* stmt->execute("DROP TABLE IF EXISTS test");*/
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
		std::cout << "not ok 1 - examples/resultset_binary.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/resultset_binary.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/resultset_binary.cpp" << std::endl;
	return EXIT_SUCCESS;
}
