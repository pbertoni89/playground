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
#include "prepared_statements.hpp"

/**
* Example of statements - not to be confused with prepared statements
*
* NOTE: The MySQL Server does not support each and every SQL statement
* to be prepared. The list of statements which can be prepared is available
* in the MySQL Server documentation and the C API documentation:
* http://dev.mysql.com/doc/refman/5.1/en/c-api-prepared-statements.html
* (Link to the MySQL Server 5.1 documentation!)
*
* Connector/C++ is based on the C API and C library "libmysql". Therefore
* it inherits all limitations from the MySQL Server and the MySQL C API.
*
* MySQL 5.1.12 can prepare the following statements:
*
* - CREATE TABLE, DELETE, DO, INSERT, REPLACE, SELECT, SET, UPDATE
* - most SHOW commands
* - ANALYZE TABLE, OPTIMIZE TABLE, REPAIR TABLE
* - CACHE INDEX, CHANGE MASTER, CHECKSUM {TABLE | TABLES},
* - {CREATE | RENAME | DROP} DATABASE, {CREATE | RENAME | DROP} USER
* - FLUSH {TABLE | TABLES | TABLES WITH READ LOCK | HOSTS | PRIVILEGES | LOGS | STATUS | MASTER | SLAVE | DES_KEY_FILE | USER_RESOURCES}
* - GRANT, REVOKE, KILL, LOAD INDEX INTO CACHE, RESET {MASTER | SLAVE | QUERY CACHE}
* - SHOW BINLOG EVENTS, SHOW CREATE {PROCEDURE | FUNCTION | EVENT | TABLE | VIEW}
* - SHOW {AUTHORS | CONTRIBUTORS | WARNINGS | ERRORS}
* - SHOW {MASTER | BINARY} LOGS, SHOW {MASTER | SLAVE} STATUS
* - SLAVE {START | STOP}, INSTALL PLUGIN, UNINSTALL PLUGIN
*
*  ... that's pretty much every *core* SQL statement - but not USE as you'll see below.
*
* Connector/C++ does not include a prepared statement emulation
*
* @link http://dev.mysql.com/doc/refman/5.1/en/c-api-prepared-statements.html
*/


int main_prepared_statement(int argc, const char **argv)
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
	std::stringstream msg;
	int i, num_rows;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;;
	std::cout << "# Connector/C++ prepared statement example.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));

		/* The usage of USE is not supported by the prepared statement protocol */
		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("USE " + database);

		/*
		Prepared statement are unhandy for queries which you execute only once!

		prepare() will send your SQL statement to the server. The server
		will do a SQL syntax check, perform some static rewriting like eliminating
		dead expressions such as "WHERE 1=1" and simplify expressions
		like "WHERE a > 1 AND a > 2" to "WHERE a > 2". Then control gets back
		to the client and the server waits for execute() (or close()).
		On execute() another round trip to the server is done.

		In case you execute your prepared statement only once - like shown below -
		you get two round trips. But using "simple" statements - like above - means
		only one round trip.

		Therefore, the below is *bad* style. WARNING: Although its *bad* style,
		the example program will continue to do it to demonstrate the (ab)use of
		prepared statements (and to prove that you really can do more than SELECT with PS).
		*/
		boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("DROP TABLE IF EXISTS test"));
		prep_stmt->execute();

		prepare_execute(con, "CREATE TABLE test(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, label CHAR(1))");
		std::cout << "#\t Test table created" << std::endl;

		/*
		The first useful example - prepare() once, execute() n + 1 times
		NOTE: The MySQL Server does not support named parameters. You have to use
		the placeholder syntax shown below. There is no emulation which would you
		allow to use named parameter like ':param1'. Use '?'. Parameters are 1-based.
		*/
		num_rows = 0;
		prep_stmt.reset(con->prepareStatement("INSERT INTO test(id, label) VALUES (?, ?)"));
		for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
			prep_stmt->setInt(1, test_data[i].id);
			prep_stmt->setString(2, test_data[i].label);
			/* executeUpdate() returns the number of affected = inserted rows */
			num_rows += prep_stmt->executeUpdate();
		}

		if (EXAMPLE_NUM_TEST_ROWS != num_rows) {
			msg.str("");
			msg << "Expecting " << EXAMPLE_NUM_TEST_ROWS << "rows, reported " << num_rows;
			throw std::runtime_error(msg.str());
		}
		std::cout << "#\t Test table populated" << std::endl;

		/* We will reuse the SELECT a bit later... */
		boost::scoped_ptr< sql::PreparedStatement > prep_select(con->prepareStatement("SELECT id, label FROM test ORDER BY id ASC"));
		std::cout << "#\t Running 'SELECT id, label FROM test ORDER BY id ASC'" << std::endl;
		boost::scoped_ptr< sql::ResultSet > res(prep_select->executeQuery());
		row = 0;
		while (res->next()) {
			std::cout << "#\t\t Row " << row << " - id = " << res->getInt("id");
			std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
			row++;
		}
		res.reset(NULL);

		if (EXAMPLE_NUM_TEST_ROWS != row) {
			msg.str("");
			msg << "Expecting " << EXAMPLE_NUM_TEST_ROWS << "rows, reported " << row;
			throw std::runtime_error(msg.str());
		}

		std::cout << "#\t Simple PS 'emulation' for USE and another SELECT" << std::endl;
		stmt.reset(emulate_prepare_execute(con, std::string("USE " + database).c_str()));
		stmt.reset(emulate_prepare_execute(con, std::string("USE " + database).c_str()));

		stmt.reset(emulate_prepare_execute(con, "SELECT id FROM test ORDER BY id ASC"));
		res.reset(stmt->getResultSet());
		if (res.get() != NULL) {
			row = 0;
			while (res->next()) {
				std::cout << "#\t\t Row " << row << " - id = " << res->getInt("id") << std::endl;
				row++;
			}
			res.reset(NULL);
		}
		stmt.reset(NULL);

		/* Running the SELECT again but fetching in reverse order */
		std::cout << "#\t SELECT and fetching in reverse order" << std::endl;

		res.reset(prep_select->executeQuery());
		row = res->rowsCount();
		std::cout << "#\t\t res->getRowsCount() = " << res->rowsCount() << std::endl;
		if (res->rowsCount() != EXAMPLE_NUM_TEST_ROWS) {
			msg.str("");
			msg << "Expecting " << EXAMPLE_NUM_TEST_ROWS << "rows, found " << res->rowsCount();
			throw std::runtime_error(msg.str());
		}

		/* Position the cursor after the last row */
		std::cout << "#\t\t Position the cursor after the last row\n";
		res->afterLast();
		std::cout << "#\t\t res->isafterLast()\t= " << res->isAfterLast() << std::endl;
		std::cout << "#\t\t res->isLast()\t\t= " << res->isLast() << std::endl;
		if (!res->isAfterLast() || res->isLast())
			throw std::runtime_error("Moving the cursor after the last row failed");

		while (res->previous()) {
			std::cout << "#\t\t res->previous()\n";
			std::cout << "#\t\t Row " << row << " - id = " << res->getInt("id");
			std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
			row--;
		}
		std::cout << "#\t\t Should be before the first\n";
		std::cout << "#\t\t res->isFirst()\t\t= " << res->isFirst() << std::endl;
		std::cout << "#\t\t res->isBeforeFirst()\t= " << res->isBeforeFirst() << std::endl;
		if (res->isFirst() || !res->isBeforeFirst())
			throw std::runtime_error("Cursor should be before first row");

		/* Now that the cursor is before the first, fetch the first */
		std::cout << "#\t\t Now that the cursor is before the first, fetch the first\n";
		std::cout << "#\t\t calling next() to fetch first row" << std::endl;
		row++;
		res->next();
		std::cout << "#\t\t res->isFirst()\t\t= " << res->isFirst() << std::endl;
		std::cout << "#\t\t Row " << row << " - id = " << res->getInt("id");
		std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		row--;

		/* For more on cursors see resultset.cpp example */

		/* Clean up */
		res.reset(NULL);
		stmt.reset(con->createStatement());
		stmt->execute("DROP TABLE IF EXISTS test");
		std::cout << "#done!" << std::endl;

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
		/* Use what(), getErrorCode() and getSQLState() */
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
		std::cout << "not ok 1 - examples/prepared_statement.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << std::endl;
		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/prepared_statement.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/prepared_statement.cpp" << std::endl;
	return EXIT_SUCCESS;
}


bool prepare_execute(boost::scoped_ptr< sql::Connection > & con, const char *sql)
{
	sql::PreparedStatement * prep_stmt;

	prep_stmt = con->prepareStatement(sql);
	prep_stmt->execute();
	delete prep_stmt;

	return true;
}


sql::Statement* emulate_prepare_execute(boost::scoped_ptr< sql::Connection > & con, const char *sql)
{
	sql::PreparedStatement *prep_stmt;
	sql::Statement *stmt = NULL;

	std::cout << "#\t\t 'emulation': " << sql << std::endl;

	try {

		prep_stmt = con->prepareStatement(sql);
		prep_stmt->execute();
		std::cout << "#\t\t 'emulation': use of sql::PreparedStatement possible" << std::endl;
		/* safe upcast - PreparedStatement is derived from Statement */
		stmt = prep_stmt;

	} catch (sql::SQLException &e) {
		/*
		Maybe the command is not supported by the MySQL Server?

		http://dev.mysql.com/doc/refman/5.1/en/error-messages-server.html
		Error: 1295 SQLSTATE: HY000 (ER_UNSUPPORTED_PS)

		Message: This command is not supported in the prepared statement protocol yet
		*/

		if (e.getErrorCode() != 1295) {
			// The MySQL Server should be able to prepare the statement
			// but something went wrong. Let the caller handle the error.
			throw ;
		}
		std::cout << "#\t\t 'emulation': ER_UNSUPPORTED_PS and fallback to sql::Statement" << std::endl;
		std::cout << "#\t\t ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		stmt = con->createStatement();
		stmt->execute(sql);
	}

	return stmt;
}
