/*
Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.

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
#include "standalone_example_docs2.hpp"


int main_standalone_example_docs2(void)
{
	std::cout << std::endl;
	std::cout << "Let's have MySQL count from 10 to 1..." << std::endl;

	try {
	  sql::Driver *driver;
	  sql::Connection *con;
	  sql::Statement *stmt;
	  sql::ResultSet *res;
	  sql::PreparedStatement *pstmt;

	  /* Create a connection */
	  driver = sql::mysql::get_driver_instance();
	  con = driver->connect(EXAMPLE_HOST, EXAMPLE_USER, EXAMPLE_PASS);
	  /* Connect to the MySQL test database */
	  con->setSchema(EXAMPLE_DB);

	  stmt = con->createStatement();
	  stmt->execute("DROP TABLE IF EXISTS test");
	  stmt->execute("CREATE TABLE test(id INT)");
	  delete stmt;

	  /* '?' is the supported placeholder syntax */
	  pstmt = con->prepareStatement("INSERT INTO test(id) VALUES (?)");
	  for (int i = 1; i <= 10; i++) {
		pstmt->setInt(1, i);
		pstmt->executeUpdate();
	  }
	  delete pstmt;

	  /* Select in ascending order */
	  pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
	  res = pstmt->executeQuery();

	  /* Fetch in reverse = descending order! */
	  res->afterLast();
	  while (res->previous())
		std::cout << "\t... MySQL counts: " << res->getInt("id") << std::endl;
	  delete res;

	  delete pstmt;
	  delete con;

	} catch (sql::SQLException &e) {
	  std::cout << "# ERR: SQLException in " << __FILE__;
	  std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
	  std::cout << "# ERR: " << e.what();
	  std::cout << " (MySQL error code: " << e.getErrorCode();
	  std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
