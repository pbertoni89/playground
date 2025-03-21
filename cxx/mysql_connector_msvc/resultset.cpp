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
#include "resultset.hpp"

/* *
* Basic example demonstrating scrolling through a result set
*
*/


int main_resultset(int argc, const char **argv)
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
		stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
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
			sql << "INSERT INTO test(id, label) VALUES (";
			sql << test_data[i].id << ", '" << test_data[i].label << "')";
			stmt->execute(sql.str());
		}
		std::cout << "#\t Test table populated" << std::endl;

		/*
		This is an example how to fetch in reverse order using the ResultSet cursor.
		Every ResultSet object maintains a cursor, which points to its current
		row of data. The cursor is 1-based. The first row has the cursor position 1.

		NOTE: The Connector/C++ preview/alpha uses buffered results.
		THe driver will always fetch all data no matter how big the result set is!
		*/
		std::cout << "#\t Testing sql::Statement based resultset" << std::endl;
		{
			boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id, label FROM test ORDER BY id ASC"));
			validateResultSet(res, &min, &max);
		}

		std::cout << "#" << std::endl;
		std::cout << "#\t Testing sql::PreparedStatment based resultset" << std::endl;

		{
			boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("SELECT id, label FROM test ORDER BY id ASC"));
			boost::scoped_ptr< sql::ResultSet > res(prep_stmt->executeQuery());
			validateResultSet(res, &min, &max);
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
		std::cout << "not ok 1 - examples/resultset.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/resultset.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << "ok 1 - examples/resultset.cpp" << std::endl;
	return EXIT_SUCCESS;
}

static void validateRow(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *exp)
{
	std::stringstream msg;

	std::cout << "#\t\t Fetching the first row, id = " << res->getInt("id");
	std::cout << ", label = '" << res->getString("label") << "'" << std::endl;

	if ((res->getInt("id") != exp->id) || (res->getString("label") != exp->label)) {
		msg.str("Wrong results");
		msg << "Expected (" << exp->id << "," << exp->label << ")";
		msg << " got (" << res->getInt("id") <<", " << res->getString("label") << ")";
		throw std::runtime_error(msg.str());
	}
}

static void validateResultSet(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *min, struct _test_data *max) {

	size_t row;

	std::cout << "#\t Selecting in ascending order but fetching in descending (reverse) order" << std::endl;
	/* Move the cursor after the last row - n + 1 */
	res->afterLast();
	if (true != res->isAfterLast())
		throw std::runtime_error("Position should be after last row (1)");

	row = res->rowsCount() - 1;
	/* Move the cursor backwards to: n, n - 1, ... 1, 0. Return true if rows are available. */
	while (res->previous()) {
		std::cout << "#\t\t Row " << row << " id = " << res->getInt("id");
		std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		row--;
	}
	/*
	The last call to res->previous() has moved the cursor before the first row
	Cursor position is 0, recall: rows are from 1 ... n
	*/
	std::cout << "#\t\t isBeforeFirst() = " << res->isBeforeFirst() << std::endl;
	std::cout << "#\t\t isFirst() = " << res->isFirst() << std::endl;
	if (!res->isBeforeFirst()) {
		throw std::runtime_error("Cursor should be positioned before the first row");
	}
	/* Move the cursor forward again to position 1 - the first row */
	res->next();
	std::cout << "#\t Positioning cursor to 1 using next(), isFirst() = " << res->isFirst() << std::endl;
	validateRow(res, min);
	/* Move the cursor to position 0 = before the first row */
	if (false != res->absolute(0)) {
		throw std::runtime_error("Call did not fail although its not allowed to move the cursor before the first row");
	}
	std::cout << "#\t Positioning before first row using absolute(0), isFirst() = " << res->isFirst() << std::endl;
	/* Move the cursor forward to position 1 = the first row */
	res->next();
	validateRow(res, min);

	/* Move the cursor to position 0 = before the first row */
	res->beforeFirst();
	std::cout << "#\t Positioning cursor using beforeFirst(), isFirst() = " << res->isFirst() << std::endl;
	/* Move the cursor forward to position 1 = the first row */
	res->next();
	std::cout << "#\t\t Moving cursor forward using next(), isFirst() = " << res->isFirst() << std::endl;
	validateRow(res, min);

	std::cout << "#\t Finally, reading in descending (reverse) order again" << std::endl;
	/* Move the cursor after the last row - n + 1 */
	res->afterLast();
	row = res->rowsCount() - 1;
	/* Move the cursor backwards to: n, n - 1, ... 1, 0. Return true if rows are available.*/
	while (res->previous()) {
		std::cout << "#\t\t Row " << row << ", getRow() " << res->getRow();
		std::cout << " id = " << res->getInt("id");
		std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		row--;
	}
	/*
	The last call to res->previous() has moved the cursor before the first row
	Cursor position is 0, recall: rows are from 1 ... n
	*/
	std::cout << "#\t\t isBeforeFirst() = " << res->isBeforeFirst() << std::endl;
	if (true != res->isBeforeFirst()) {
		throw std::runtime_error("Position should be 0 = before first row");
	}

	std::cout << "#\t And in regular order..." << std::endl;
	res->beforeFirst();
	if (true != res->isBeforeFirst()) {
		throw std::runtime_error("Cursor should be positioned before the first row");
	}
	row = 0;
	while (res->next()) {
		std::cout << "#\t\t Row " << row << ", getRow() " << res->getRow();
		std::cout << " id = " << res->getInt("id");
		std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
		row++;
	}
	std::cout << "#\t\t isAfterLast() = " << res->isAfterLast() << std::endl;
	if (true != res->isAfterLast()) {
		throw std::runtime_error("next() has returned false and the cursor should be after the last row");
	}
	/* Move to the last entry using a negative offset for absolute() */
	std::cout << "#\t Trying absolute(-1) to fetch last entry..." << std::endl;
	if (true != res->absolute(-1)) {
		throw std::runtime_error("Call did fail although -1 is valid");
	}
	std::cout << "#\t\t isAfterLast() = " << res->isAfterLast() << std::endl;
	if (false != res->isAfterLast()) {
		throw std::runtime_error("Cursor should be positioned to the last row and not after the last row");
	}
	std::cout << "#\t\t isLast() = " << res->isLast() << std::endl;
	if (true != res->isLast()) {
		throw std::runtime_error("Cursor should be positioned to the last row");
	}
	validateRow(res, max);
	/* Another way to move after the last entry */
	std::cout << "#\t Trying absolute(NUMROWS + 10) to move cursor after last row and fetch last entry..." << std::endl;
	if (false != res->absolute(res->rowsCount() + 10)) {
		throw std::runtime_error("Call did fail although parameter is valid");
	}
	if (true != res->isAfterLast()) {
		throw std::runtime_error("Cursor should be positioned after the last row");
	}
	std::cout << "#\t\t isLast() = " << res->isLast() << std::endl;
	if (false != res->isLast()) {
		throw std::runtime_error("Cursor should be positioned after the last row");
	}
	try {
		res->getString(1);
		throw std::runtime_error("Fetching is possible although cursor is out of range");
	} catch (sql::InvalidArgumentException &) {
		std::cout << "#\t\t OK, fetching not allowed when cursor is out of range..." << std::endl;
	}
	/* absolute(NUM_ROWS + 10) is internally aligned to NUM_ROWS + 1 = afterLastRow() */
	res->previous();
	validateRow(res, max);
}
