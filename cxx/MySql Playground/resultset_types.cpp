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
#include "resultset_types.hpp"

/* *
* Basic example demonstrating how to check the type of a result set column
*
*/


int main_resultset_types(int argc, const char **argv)
{
	const std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	/* Driver Manager */
	sql::Driver *driver;

	struct _test_data min, max;
	int c_int1;
	bool c_bool1 = true, c_bool2;
	/* TODO: long long is not C++, its C99 !!! */
	int64_t c_long1 = L64(9223372036854775807), c_long2;
	double c_double1 = -999.9999, c_double2;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ result set.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));
		con->setSchema(database);

		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("DROP TABLE IF EXISTS test");

		/*
		Note that MySQL has its very own mapping from SQL type (e.g. BOOLEAN)
		specified in a SQL statement and type actually used. Check the MySQL
		manual - conversions are a common cause of false bug reports!
		Also, don't get confused by the precision of float/double columns.
		For precision math use DECIMAL!
		*/
		stmt->execute("CREATE TABLE test(id INT, label CHAR(1), c_bool BOOLEAN, "
			"c_long BIGINT, c_double DOUBLE, c_null INT DEFAULT NULL)");
		std::cout << "#\t Test table created" << std::endl;

		boost::scoped_ptr< sql::PreparedStatement> prep_stmt(
			con->prepareStatement("INSERT INTO test(id, label, c_bool, c_long, "
				" c_double) VALUES (?, ?, ?, ?, ?)"));

		/* Populate the test table with data */
		min = max = test_data[0];
		for (unsigned int i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			/* Remember min/max for further testing */
			if (test_data[i].id < min.id) {
				min = test_data[i];
			}
			if (test_data[i].id > max.id) {
				max = test_data[i];
			}

			prep_stmt->setInt(1, test_data[i].id);
			prep_stmt->setString(2, test_data[i].label);
			prep_stmt->setBoolean(3, c_bool1);
			prep_stmt->setInt64(4, c_long1);
			prep_stmt->setDouble(5, c_double1);
			prep_stmt->execute();
		}
		std::cout << "#\t Test table populated" << std::endl;

		boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id, label, c_bool, c_long, c_double, c_null FROM test ORDER BY id ASC"));
		while (res->next()) {
			/* sql::ResultSet.rowsCount() returns size_t */
			size_t row = res->getRow() - 1;

			std::cout << "#\t\t Row " << res->getRow() << std::endl;
			std::cout << "#\t\t\t id INT = " << res->getInt("id") << std::endl;
			std::cout << "#\t\t\t id (as Integer) = " << res->getInt("id") << std::endl;
			std::cout << "#\t\t\t id (as String) = " << res->getString("id") << std::endl;
			std::cout << "#\t\t\t id (as Boolean) = " << res->getBoolean("id") << std::endl;
			std::cout << "#\t\t\t id (as Long) = " << res->getInt64("id") << std::endl;
			std::cout << "#\t\t\t id (as Double) = " << res->getDouble("id") << std::endl;
			std::cout << "#" << std::endl;
			if (test_data[row].id != res->getInt(1)) {
				throw std::runtime_error("Wrong results for column id");
			}

			std::string c_string = res->getString(2);
			std::cout << "#\t\t\t label CHAR(1) = " << c_string << std::endl;
			std::cout << "#\t\t\t label (as Integer) = " << res->getInt(2) << std::endl;
			std::cout << "#\t\t\t label (as String) = " << res->getString(2) << std::endl;
			std::cout << "#\t\t\t label (as Boolean) = " << res->getBoolean(2) << std::endl;
			std::cout << "#\t\t\t label (as Long) = " << res->getInt64(2) << std::endl;
			std::cout << "#\t\t\t label (as Double) = " << res->getDouble(2) << std::endl;
			std::cout << "#" << std::endl;
			if (test_data[row].label != c_string) {
				throw std::runtime_error("Wrong result for column label");
			}

			c_bool2 = res->getBoolean("c_bool");
			std::cout << "#\t\t\t c_bool CHAR(1) = " << c_bool2 << std::endl;
			std::cout << "#\t\t\t c_bool (as Integer) = " << res->getInt(3) << std::endl;
			std::cout << "#\t\t\t c_bool (as String) = " << res->getString(3) << std::endl;
			std::cout << "#\t\t\t c_bool (as Boolean) = " << res->getBoolean(3) << std::endl;
			std::cout << "#\t\t\t c_bool (as Long) = " << res->getInt64(3) << std::endl;
			std::cout << "#\t\t\t c_bool (as Double) = " << res->getDouble(3) << std::endl;
			std::cout << "#" << std::endl;
			if (c_bool1 != c_bool2) {
				throw std::runtime_error("Wrong result for column c_bool");
			}

			c_long2 = res->getInt64("c_long");
			std::cout << "#\t\t\t c_long BIGINT = " << c_long2 << std::endl;
			std::cout << "#\t\t\t c_long (as Integer) = " << res->getInt("c_long") << std::endl;
			std::cout << "#\t\t\t c_long (as String) = " << res->getString("c_long") << std::endl;
			std::cout << "#\t\t\t c_long (as Boolean) = " << res->getBoolean("c_long") << std::endl;
			std::cout << "#\t\t\t c_long (as Long) = " << res->getInt64("c_long") << std::endl;
			std::cout << "#\t\t\t c_long (as Double) = " << res->getDouble("c_long") << std::endl;
			std::cout << "#" << std::endl;
			if (c_long1 != c_long2) {
				throw std::runtime_error("Wrong result for column c_long");
			}

			c_double2 = res->getDouble("c_double");
			std::cout << "#\t\t\t c_double DOUBLE = " << c_double2 << std::endl;
			std::cout << "#\t\t\t c_double (as Integer) = " << res->getInt("c_double") << std::endl;
			std::cout << "#\t\t\t c_double (as String) = " << res->getString("c_double") << std::endl;
			std::cout << "#\t\t\t c_double (as Boolean) = " << res->getBoolean("c_double") << std::endl;
			std::cout << "#\t\t\t c_double (as Long) = " << res->getInt64("c_double") << std::endl;
			std::cout << "#\t\t\t c_double (as Double) = " << res->getDouble("c_double") << std::endl;
			std::cout << "#\t\t\t c_double wasNull() = " << res->wasNull() << std::endl;
			std::cout << "#" << std::endl;
			if (c_double1 != c_double2) {
				throw std::runtime_error("Wrong result for column c_double");
			}

			c_int1 = res->getInt("c_null");
			std::cout << "#\t\t\t c_null INT DEFAULT NULL = " << c_int1;
			std::cout << " (isNull = " << res->isNull("c_null") << ")" << std::endl;
			std::cout << "#\t\t\t c_null (as Integer) = " << res->getInt("c_null") << std::endl;
			std::cout << "#\t\t\t c_null (as String) = " << res->getString("c_null") << std::endl;
			std::cout << "#\t\t\t c_null (as Boolean) = " << res->getBoolean("c_null") << std::endl;
			std::cout << "#\t\t\t c_null (as Long) = " << res->getInt64("c_null") << std::endl;
			std::cout << "#\t\t\t c_null (as Double) = " << res->getDouble("c_null") << std::endl;
			std::cout << "#\t\t\t c_null wasNull() = " << res->wasNull() << std::endl;
			std::cout << "#" << std::endl;
			if (!res->isNull(6) || !res->wasNull()) {
				throw std::runtime_error("isNull() or wasNull() has not reported NULL value of column c_null");
			}

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
		/* Use what(), getErrorCode() and getSQLState() */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "not ok 1 - examples/resultset_types.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/resultset_types.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/resultset_types.cpp" << std::endl;
	return EXIT_SUCCESS;
}
