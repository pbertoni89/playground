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
#include "exceptions.hpp"

/**
* Types of execptions thrown by Connector/C++
*
*/


int main_exceptions(int argc, const char **argv)
{
	static const std::string host(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	static const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	static const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	static const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	// Driver Manager
	sql::Driver *driver;
	int i;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ types of exceptions.." << std::endl;

	try {
		/* Using the Driver to create a connection */
		driver = sql::mysql::get_driver_instance();
		boost::scoped_ptr< sql::Connection > con(driver->connect(host, user, pass));

		/* Run in autocommit mode */
		con->setAutoCommit(1);
		boost::scoped_ptr< sql::Savepoint > savepoint(NULL);
		try {
			// It makes no sense to set a savepoint in autocommit mode
			savepoint.reset(con->setSavepoint(std::string("before_insert")));
			throw new std::runtime_error("Setting a savepoint should not be allow in autocommit mode");
		} catch (sql::InvalidArgumentException &e) {
			std::cout << "#\t Invalid Argument: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		con->setSchema(database);

		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, label CHAR(1))");
		std::cout << "#\t Test table created" << std::endl;

		try {
			con->setReadOnly(true);
		} catch (sql::MethodNotImplementedException &e) {
			std::cout << "#\t Not implemented: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		con->setAutoCommit(0);
		try {
			savepoint.reset(con->setSavepoint(std::string("")));
		} catch (sql::InvalidArgumentException &e) {
			std::cout << "#\t Invalid Argument: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}
		savepoint.reset(con->setSavepoint(std::string("before_insert")));

		{
			boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("INSERT INTO test(id, label) VALUES (?, ?)"));
			for (i = 0; i < EXAMPLE_NUM_TEST_ROWS; i++) {
				prep_stmt->setInt(1, test_data[i].id);
				prep_stmt->setString(2, test_data[i].label);
				prep_stmt->executeUpdate();
			}
		}

		try {
			boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("INSERT INTO test(id, label) VALUES (?, ?)"));
			prep_stmt->setInt(1, test_data[0].id);
			/* This will cause a duplicate index error */
			prep_stmt->executeUpdate();
			throw new std::runtime_error("Query not executed or duplicate key not detected");
		} catch (sql::SQLException &e) {
			std::cout << "#\t SQL Exception: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		/* Switching back to auto commit will invalidate the savepoint!	*/
		con->setAutoCommit(1);
		try {
			con->releaseSavepoint(savepoint.get());
			throw new std::runtime_error("Releasing a savepoint should not be allow in autocommit mode");
		} catch (sql::InvalidArgumentException &e) {
			std::cout << "#\t Invalid Argument: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}

		stmt->execute("DROP TABLE IF EXISTS test");
		std::cout << "#done!" << std::endl;

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)

                 sql::SQLException is the base class.
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
		std::cout << "not ok 1 - examples/exceptions.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << std::endl;
		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/exceptions.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/exceptions.cpp" << std::endl;
	return EXIT_SUCCESS;
}


bool prepare_execute(sql::Connection *con, const char *sql)
{
	sql::PreparedStatement *prep_stmt;

	prep_stmt = con->prepareStatement(sql);
	prep_stmt->execute();
	delete prep_stmt;

	return true;
}


sql::Statement* emulate_prepare_execute(sql::Connection *con, const char *sql)
{
	sql::PreparedStatement *prep_stmt;
	sql::Statement *stmt = NULL;

	std::cout << "#\t\t 'emulation': " << sql << std::endl;

	try {

		prep_stmt = con->prepareStatement(sql);
		prep_stmt->execute();
		std::cout << "#\t\t 'emulation': use of sql::PreparedStatement possible" << std::endl;
		// safe upcast - PreparedStatement is derived from Statement
		stmt = prep_stmt;

	} catch (sql::SQLException &e) {
		/*
		Maybe the command is not supported by the MySQL Server?

		http://dev.mysql.com/doc/refman/5.1/en/error-messages-server.html
		Error: 1295 SQLSTATE: HY000 (ER_UNSUPPORTED_PS)

		Message: This command is not supported in the prepared statement protocol yet
		*/
		if (e.getErrorCode() != 1295) {
			/*
			The MySQL Server should be able to prepare the statement
			but something went wrong. Let the caller handle the error.
			*/
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
