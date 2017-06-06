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
#include "connection_meta_schemaobj.hpp"


/**
* Usage example for Driver, Connection, (simple) Statement, ResultSet
*/
int main_connection_meta_schemaobj(int argc, const char **argv)
{

	std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	static std::list< sql::SQLString > table_types;
	table_types.push_back("TABLE");

	unsigned int column, row;
	std::string ddl;

	std::cout << std::boolalpha;
	std::cout << "1..1" << std::endl;
	std::cout << "# Connector/C++ connection meta data example.." << std::endl;
	std::cout << "#" << std::endl;


	try {
		/* Using the Driver to create a connection */
		sql::Driver * driver = sql::mysql::get_driver_instance();
		std::cout << "# " << driver->getName() << ", version ";
		std::cout << driver->getMajorVersion() << "." << driver->getMinorVersion();
                std::cout << "." << driver->getPatchVersion() << std::endl;

		boost::scoped_ptr< sql::Connection > con(driver->connect(url, user, pass));
		sql::DatabaseMetaData * con_meta = con->getMetaData();

		std::cout << "# CDBC (API) major version = " << con_meta->getCDBCMajorVersion() << std::endl;
		if (con_meta->getCDBCMajorVersion() <= 0) {
			throw std::runtime_error("API major version must not be 0");
		}
		std::cout << "# CDBC (API) minor version = " << con_meta->getCDBCMinorVersion() << std::endl;

		{
			/*
			Which schemata/databases exist?
			NOTE Connector C++ defines catalog = n/a, schema = MySQL database
			*/
			std::cout << "# List of available schemata/databases: ";
			boost::scoped_ptr< sql::ResultSet > res(con_meta->getSchemas());

			/* just for fun... of course you can scroll and fetch in whatever order you want */
			res->afterLast();
			while (res->previous()) {
				std::cout << res->getString("TABLE_SCHEM");
				if (!res->isFirst()) {
					std::cout << ", ";
				}
			}
			std::cout << std::endl;
			if (!res->isBeforeFirst() || res->isFirst())
				throw std::runtime_error("Cursor should be positioned before first row");
		}

		{
			/* What object types does getSchemaObjects support? */
			std::cout << "# Supported Object types: ";
			boost::scoped_ptr< sql::ResultSet > res(con_meta->getSchemaObjectTypes());
			while (res->next()) {
				std::cout << res->getString(1);
				if (!res->isLast()) {
					std::cout << ", ";
				}
			}
			std::cout << std::endl;
			if (!res->isAfterLast() || res->isLast())
				throw std::runtime_error("Cursor should be positioned after last row");
		}


		boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
		stmt->execute("USE " + database);
		stmt->execute("DROP TABLE IF EXISTS test1");
		stmt->execute("CREATE TABLE test1(id INT, label CHAR(1))");
		stmt->execute("DROP TABLE IF EXISTS test2");
		stmt->execute("CREATE TABLE test2(id INT, label CHAR(1))");

		/* "" = empty std::string requests all types of objects */
		boost::scoped_ptr< sql::ResultSet > res(con_meta->getSchemaObjects(con->getCatalog(), con->getSchema(), ""));
		row = 1;
		sql::ResultSetMetaData * res_meta = res->getMetaData();
		while (res->next()) {
			if (row++ > 2)
				break;

			std::cout << "#\t Object " << res->getRow() << " (" << res->getString("OBJECT_TYPE") << ")" << std::endl;
			if (res->getString(1) != res->getString("OBJECT_TYPE")) {
				throw std::runtime_error("Fetch using numeric/non-numeric index failed for OBJECT_TYPE");
			}
			if (res->getString("OBJECT_TYPE").length() == 0) {
				throw std::runtime_error("OBJECT_TYPE must not be empty");
			}

			std::cout << "#\t Catalog = " << res->getString("CATALOG");
			std::cout << " (empty: " << (res->getString("CATALOG").length() == 0) << ")" << std::endl;
			if (res->getString(2) != res->getString("CATALOG")) {
				throw std::runtime_error("Fetch using numeric/non-numeric index failed for CATALOG");
			}

			std::cout << "#\t Schema = " << res->getString("SCHEMA");
			std::cout << " (empty: " << (res->getString("SCHEMA").length() == 0) << ")" << std::endl;
			if (res->getString(3) != res->getString("SCHEMA")) {
				throw std::runtime_error("Fetch using numeric/non-numeric index failed for SCHEMA");
			}

			std::cout << "#\t Name = " << res->getString("NAME");
			std::cout << " (empty: " << (res->getString("NAME").length() == 0) << ")" << std::endl;
			if (res->getString(4) != res->getString("NAME")) {
				throw std::runtime_error("Fetch using numeric/non-numeric index failed for NAME");
			}

			ddl = res->getString("DDL");
			std::cout << "#\t DDL = " << std::setw(40);
			std::cout << ddl.substr(0, ddl.find_first_of("\n", 1) - 1) << "..." << std::endl;
			std::cout << "#\t DDL is empty: " << ddl.empty() << std::endl;
			if (res->getString(5) != res->getString("DDL")) {
				throw std::runtime_error("Fetch using numeric/non-numeric index failed for DDL");
			}

		}
		std::cout << "#" << std::endl;
		std::cout << "#\t\t Using different getter methods at the example of the DDL column" << std::endl;
		std::cout << "#\t\t Column DDL is of type " << res_meta->getColumnTypeName(5);
		std::cout << " / Code: " << res_meta->getColumnType(5) << std::endl;
		/* scroll back to last row in set */
		res->previous();
		std::cout << "#\t\t DDL (as String) = " << std::setw(30) << ddl.substr(0, ddl.find_first_of("\n", 1) - 1) << "..." << std::endl;
		std::cout << "#\t\t DDL (as Boolean) = " << res->getBoolean("DDL") << "/" << res->getBoolean(5) << std::endl;
		std::cout << "#\t\t DDL (as Double) = " << res->getDouble("DDL") << "/" << res->getDouble(5) << std::endl;
		std::cout << "#\t\t DDL (as Int) = " << res->getInt("DDL") << "/" << res->getInt(5) << std::endl;
		std::cout << "#\t\t DDL (as Long) = " << res->getInt64("DDL") << "/" << res->getInt64(5) << std::endl;
		std::cout << "#" << std::endl;

		std::cout << "#\t\t Meta data on the result set at the example of the DDL column" << std::endl;
		std::cout << "#\t\t Column count = " << res_meta->getColumnCount() << " (Columns: ";
		for (column = 1; column <= res_meta->getColumnCount(); column++) {
			std::cout << res_meta->getColumnName(column);
			if (column < res_meta->getColumnCount()) {
				std::cout << ", ";
			}
		}
		std::cout << ")" << std::endl;

		std::cout << "#\t\t getCatalogName() = " << res_meta->getCatalogName(5) << std::endl;
		/* std::cout << "#\t\t getColumnDisplaySize() = " << res_meta->getDisplaySize("DDL") << std::endl; */
		std::cout << "#\t\t getColumnLabel() = " << res_meta->getColumnLabel(5) << std::endl;
		std::cout << "#\t\t getColumnName() = " << res_meta->getColumnName(5) << std::endl;
		if (res_meta->getColumnName(5) != res_meta->getColumnLabel(5)) {
			throw std::runtime_error("column names differ for column DDL");
		}

		std::cout << "#\t\t getColumnType() = " << res_meta->getColumnType(5) << std::endl;
		std::cout << "#\t\t getColumnTypeName() " << res_meta->getColumnTypeName(5) << std::endl;
		/* std::cout << "#\t\t getPrecision() = " << res_meta->getPrecision(5) << std::endl; */
		std::cout << "#\t\t getSchemaName() = " << res_meta->getSchemaName(5) << std::endl;
		std::cout << "#\t\t getTableName() = " << res_meta->getTableName(5) << std::endl;
		std::cout << "#\t\t isAutoIncrement() = " << res_meta->isAutoIncrement(5) << std::endl;
		std::cout << "#\t\t isCaseSensitive() = " << res_meta->isCaseSensitive(5) << std::endl;
		std::cout << "#\t\t isCurrency() = " << res_meta->isCurrency(5) << std::endl;
		std::cout << "#\t\t isDefinitelyWritable() = " << res_meta->isDefinitelyWritable(5) << std::endl;
		std::cout << "#\t\t isNullable() = " << res_meta->isNullable(5) << std::endl;
		std::cout << "#\t\t isReadOnly() = " << res_meta->isReadOnly(5) << std::endl;
		std::cout << "#\t\t isSearchable() = " << res_meta->isSearchable(5) << std::endl;
		std::cout << "#\t\t isSigned() = " << res_meta->isSigned(5) << std::endl;
		std::cout << "#\t\t isWritable() = " << res_meta->isWritable(5) << std::endl;

		boost::scoped_ptr< sql::ResultSet > res_tables(con_meta->getTables(con->getCatalog(), database, "t%", table_types));
		sql::ResultSetMetaData * res_meta_tables = res_tables->getMetaData();

		std::cout << "#" << std::endl;
		std::cout << "# Tables with names like 't%':" << std::endl;;
		std::cout << "#\t rowsCount() = " << res_tables->rowsCount() << std::endl;
		std::cout << "#\t getColumnCount() = " << res_meta_tables->getColumnCount() << std::endl;
		std::cout << "#\t" << std::endl;
		while (res_tables->next()) {
			std::cout << "#\t ";
			for (column = 1; column < res_meta_tables->getColumnCount(); column++) {
				std::cout << "'" << res_tables->getString(column) << "' ";
			}
			std::cout <<  std::endl;
		}
		std::cout << "#" << std::endl;

	  	res_tables->first();
		boost::scoped_ptr< sql::ResultSet > res_columns(con_meta->getColumns(con->getCatalog(), database, "test1", "%"));

		std::cout << "#" << "Columns in the table 'test1'..." << std::endl;
		std::cout << "#\t rowsCount() = " << res_columns->rowsCount() << std::endl;
		if (res_columns->rowsCount() != 2)
			throw std::runtime_error("Table test1 has two columns, no more!");

		while (res_columns->next()) {
			std::cout << "#\t Column name = '" << res_columns->getString("COLUMN_NAME") << "'" << std::endl;
		}
		std::cout << "#" << std::endl;

		std::cout << "# done!" << std::endl;
		std::cout << "ok 1 - examples/connection_meta_schemaobj.cpp" << std::endl;

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		/* Use what() (derived from std::runtime_error) */
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "not ok 1 - examples/connection_meta_schemaobj.cpp" << std::endl;

		return EXIT_FAILURE;
	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/connection_meta_schemaobj.cpp" << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
