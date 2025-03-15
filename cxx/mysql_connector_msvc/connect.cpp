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
#include "connect.hpp"

/**
* Basic example demonstrating connect and simple queries
*
*/


/**
* Usage example for Driver, Connection, (simple) Statement, ResultSet
*/
int main_connect(int argc, const char **argv)
{
	std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	/* sql::ResultSet.rowsCount() returns size_t */
	size_t row;
	std::stringstream sql;
	std::stringstream msg;
	int i, affected_rows;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ connect basic usage example.." << std::endl;
	std::cout << "#" << std::endl;

	try {
		sql::Driver * driver = sql::mysql::get_driver_instance();
		/* Using the Driver to create a connection */
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));

		/* Creating a "simple" statement - "simple" = not a prepared statement */
		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());

		/* Create a test table demonstrating the use of sql::Statement.execute() */
		stmt->execute("USE " + database);
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
		std::cout << "#\t Test table created" << std::endl;

		/* Populate the test table with data */
		for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			/*
			KLUDGE: You should take measures against SQL injections!
			example.h contains the test data
			*/
			sql.str("");
			sql << "INSERT INTO test(id, label) VALUES (";
			sql << test_data[i].id << ", '" << test_data[i].label << "')";
			stmt->execute(sql.str());
		}
		std::cout << "#\t Test table populated" << std::endl;

		{
			/*
			Run a query which returns exactly one result set like SELECT
			Stored procedures (CALL) may return more than one result set
			*/
			boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id, label FROM test ORDER BY id ASC"));
			std::cout << "#\t Running 'SELECT id, label FROM test ORDER BY id ASC'" << std::endl;

			/* Number of rows in the result set */
			std::cout << "#\t\t Number of rows\t";
			std::cout << "res->rowsCount() = " << res->rowsCount() << std::endl;
			if (res->rowsCount() != EXAMPLE_NUM_TEST_ROWS) {
				msg.str("");
				msg << "Expecting " << EXAMPLE_NUM_TEST_ROWS << "rows, found " << res->rowsCount();
				throw std::runtime_error(msg.str());
			}

			/* Fetching data */
			row = 0;
			while (res->next()) {
				std::cout << "#\t\t Fetching row " << row << "\t";
				/* You can use either numeric offsets... */
				std::cout << "id = " << res->getInt(1);
				/* ... or column names for accessing results. The latter is recommended. */
				std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
				row++;
			}
		}

		{
			/* Fetching again but using type convertion methods */
			boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id FROM test ORDER BY id DESC"));
			std::cout << "#\t Fetching 'SELECT id FROM test ORDER BY id DESC' using type conversion" << std::endl;
			row = 0;
			while (res->next()) {
				std::cout << "#\t\t Fetching row " << row;
				std::cout << "#\t id (int) = " << res->getInt("id");
				std::cout << "#\t id (boolean) = " << res->getBoolean("id");
				std::cout << "#\t id (long) = " << res->getInt64("id") << std::endl;
				row++;
			}
		}

		/* Usage of UPDATE */
		stmt->execute("INSERT INTO test(id, label) VALUES (100, 'z')");
		affected_rows = stmt->executeUpdate("UPDATE test SET label = 'y' WHERE id = 100");
		std::cout << "#\t UPDATE indicates " << affected_rows << " affected rows" << std::endl;
		if (affected_rows != 1) {
			msg.str("");
			msg << "Expecting one row to be changed, but " << affected_rows << "change(s) reported";
			throw std::runtime_error(msg.str());
		}

		{
			boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id, label FROM test WHERE id = 100"));

			res->next();
			if ((res->getInt("id") != 100) || (res->getString("label") != "y")) {
				msg.str("Update must have failed, expecting 100/y got");
				msg << res->getInt("id") << "/" << res->getString("label");
				throw std::runtime_error(msg.str());
			}

			std::cout << "#\t\t Expecting id = 100, label = 'y' and got id = " << res->getInt("id");
			std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		}

		/* Clean up */
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt.reset(NULL); /* free the object inside  */

		std::cout << "#" << std::endl;
		std::cout << "#\t Demo of connection URL syntax" << std::endl;
		try {
			/*s This will implicitly assume that the host is 'localhost' */
			url = "unix://path_to_mysql_socket.sock";
			con.reset(driver->connect(url, user, pass));
		} catch (sql::SQLException &e) {
			std::cout << "#\t\t unix://path_to_mysql_socket.sock caused expected exception" << std::endl;
			std::cout << "#\t\t " << e.what() << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		try {
			url = "tcp://hostname_or_ip[:port]";
			con.reset(driver->connect(url, user, pass));
		} catch (sql::SQLException &e) {
			std::cout << "#\t\t tcp://hostname_or_ip[:port] caused expected exception" << std::endl;
			std::cout << "#\t\t " << e.what() << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		try {
			/*
			Note: in the MySQL C-API host = localhost would cause a socket connection!
			Not so with the C++ Connector. The C++ Connector will translate
			tcp://localhost into tcp://127.0.0.1 and give you a TCP connection
			url = "tcp://localhost[:port]";
			*/
			con.reset(driver->connect(url, user, pass));
		} catch (sql::SQLException &e) {
			std::cout << "#\t\t tcp://hostname_or_ip[:port] caused expected exception" << std::endl;
			std::cout << "#\t\t " << e.what() << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

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
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "not ok 1 - examples/connect.php" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/connect.php" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/connect.php" << std::endl;
	return EXIT_SUCCESS;
}
