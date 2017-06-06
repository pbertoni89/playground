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
#include "resultset_meta.hpp"

/* *
* Example of sql::ResultSetMetaData - meta data of a result set
*
*/


/**
* Meta data of a (simple) statements result set - not prepared statements
*/
int main_resultset_meta(int argc, const char **argv)
{
  const std::string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
  const std::string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
  const std::string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
  const std::string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

  std::stringstream sql;
  unsigned int i;
  int ret;

  std::cout << std::boolalpha;
  std::cout << "1..1" << std::endl;
  std::cout << "# Connector/C++ resultset metadata..." << std::endl;

  try {
    /* Using the Driver to create a conection */
    boost::scoped_ptr< sql::Connection > con(sql::mysql::get_driver_instance()->connect(url, user, pass));

    con->setSchema(database);

    /* Create a test table demonstrating the use of sql::Statement.execute() */
    boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
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

    {
      boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT id AS column_alias, label FROM test AS table_alias LIMIT 1"));
      boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("SELECT id AS column_alias, label FROM test AS table_alias LIMIT 1"));
      boost::scoped_ptr< sql::ResultSet > ps_res(prep_stmt->executeQuery());
      std::cout << "#\t SELECT id AS column_alias, label FROM test AS table_alias LIMIT 1" << std::endl;
      printResultSetMetaData(res, ps_res);
    }

    {
      boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("SELECT 1.01, 'Hello world!'"));
      boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("SELECT 1.01, 'Hello world!'"));
      boost::scoped_ptr< sql::ResultSet > ps_res(prep_stmt->executeQuery());
      std::cout << "#\t SELECT 1.01, 'Hello world!'" << std::endl;
      printResultSetMetaData(res, ps_res);
    }

    {
      boost::scoped_ptr< sql::ResultSet > res(stmt->executeQuery("DESCRIBE test"));
      boost::scoped_ptr< sql::PreparedStatement > prep_stmt(con->prepareStatement("DESCRIBE test"));
      boost::scoped_ptr< sql::ResultSet > ps_res(prep_stmt->executeQuery());
      std::cout << "# \tDESCRIBE test" << std::endl;
      printResultSetMetaData(res, ps_res);
    }

    /* Clean up */
    stmt->execute("DROP TABLE IF EXISTS test");
    std::cout << "# done!" << std::endl;
    ret = EXIT_SUCCESS;

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

    ret = EXIT_FAILURE;

  } catch (std::runtime_error &e) {
    std::cout << "# ERR: std::runtime_error in " << __FILE__;
    std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;

    ret = EXIT_FAILURE;
  }

  if (ret != EXIT_SUCCESS) {
    std::cout << "not ";
  }
  std::cout << "ok 1 - examples/resultset_meta.cpp" << std::endl;
  return ret;
}


static void printResultSetMetaData(boost::scoped_ptr< sql::ResultSet > &res, boost::scoped_ptr< sql::ResultSet > &ps_res)
{
  /* ResultSetMetaData object */
  sql::ResultSetMetaData * meta;
  sql::ResultSetMetaData * ps_meta;
  unsigned int column;

  if (res->rowsCount() == 0) {
    throw std::runtime_error("FAILURE - no rows");
  }

  /* Get the meta data - we leave all the exception handling to the caller... */
  meta = res->getMetaData();
  ps_meta = ps_res->getMetaData();

  std::cout << "#\t Printing result set meta data" << std::endl;
  std::cout << "#\t res->rowsCount() = " << res->rowsCount() << std::endl;
  if (res->rowsCount() != ps_res->rowsCount())
    throw std::runtime_error("rowsCount() different for PS and non-PS");

  std::cout << "#\t meta->getColumnCount() = " << meta->getColumnCount() << std::endl;
  if (meta->getColumnCount() != ps_meta->getColumnCount())
    throw std::runtime_error("getColumnCount() different for PS and non-PS");

  /*
  Dump information for every column
  NOTE: column indexing is 1-based not zero-based!
  */
  for (column = 1; column <= meta->getColumnCount(); column++) {
    std::cout << "#\t\t Column " << column << "\t\t\t= " << meta->getColumnName(column);
    std::cout << "/" << ps_meta->getColumnName(column) << std::endl;
    if (meta->getColumnName(column) != ps_meta->getColumnName(column))
      throw std::runtime_error("getColumnName different for PS and non-PS");

    std::cout << "#\t\t meta->getCatalogName()\t\t= " << meta->getCatalogName(column);
    std::cout  << "/" << ps_meta->getCatalogName(column) << std::endl;
    if (meta->getCatalogName(column) != ps_meta->getCatalogName(column))
      throw std::runtime_error("getCatalogName different for PS and non-PS");

    std::cout << "#\t\t meta->getColumnDisplaySize() = " << meta->getColumnDisplaySize(column);
    std::cout  << "/" << ps_meta->getColumnDisplaySize(column) << std::endl;
    if (meta->getColumnDisplaySize(column) != ps_meta->getColumnDisplaySize(column))
      throw std::runtime_error("getColumnDisplaySize different for PS and non-PS");

    std::cout << "#\t\t meta->getColumnLabel()\t\t= " << meta->getColumnLabel(column);
    std::cout << "/" << ps_meta->getColumnLabel(column) << std::endl;
    if (meta->getColumnLabel(column) != ps_meta->getColumnLabel(column))
      throw std::runtime_error("getColumnLabel different for PS and non-PS");

    std::cout << "#\t\t meta->getColumnName()\t\t= " << meta->getColumnName(column);
    std::cout << "/" << ps_meta->getColumnName(column) << std::endl;
    if (meta->getColumnName(column) != ps_meta->getColumnName(column))
      throw std::runtime_error("getColumnName different for PS and non-PS");

    std::cout << "#\t\t meta->getColumnType()\t\t= " << meta->getColumnType(column);
    std::cout << "/" << ps_meta->getColumnType(column) << std::endl;
    if (meta->getColumnType(column) != ps_meta->getColumnType(column))
      throw std::runtime_error("getColumnType different for PS and non-PS");

    std::cout << "#\t\t meta->getColumnTypeName()\t= " << meta->getColumnTypeName(column);
    std::cout << "/" << ps_meta->getColumnTypeName(column) << std::endl;
    if (meta->getColumnTypeName(column) != ps_meta->getColumnTypeName(column))
      throw std::runtime_error("getColumnTypeName different for PS and non-PS");

    std::cout << "#\t\t meta->getPrecision()\t\t= " << meta->getPrecision(column);
    std::cout << "/" << ps_meta->getPrecision(column) << std::endl;

    //TODO: FIX IT - Giving different results
//    if (meta->getPrecision(column) != ps_meta->getPrecision(column))
//      throw std::runtime_error("getPrecision different for PS and non-PS");

    std::cout << "#\t\t meta->getScale()\t\t= " << meta->getScale(column);
    std::cout << "/" << ps_meta->getScale(column) << std::endl;
    if (meta->getScale(column) != ps_meta->getScale(column))
      throw std::runtime_error("getScale different for PS and non-PS");

    std::cout << "#\t\t meta->getSchemaName()\t\t= " << meta->getSchemaName(column);
    std::cout << "/" << ps_meta->getSchemaName(column) << std::endl;
    if (meta->getSchemaName(column) != ps_meta->getSchemaName(column))
      throw std::runtime_error("getSchemaName different for PS and non-PS");

    std::cout << "#\t\t meta->getTableName()\t\t= " << meta->getTableName(column);
    std::cout << "/" << ps_meta->getTableName(column) << std::endl;
    if (meta->getTableName(column) != ps_meta->getTableName(column))
      throw std::runtime_error("getTableName different for PS and non-PS");

    std::cout << "#\t\t meta->isAutoIncrement()\t= " << meta->isAutoIncrement(column);
    std::cout << "/" << ps_meta->isAutoIncrement(column) << std::endl;
    if (meta->isAutoIncrement(column) != ps_meta->isAutoIncrement(column))
      throw std::runtime_error("isAutoIncrement different for PS and non-PS");

    std::cout << "#\t\t meta->isCaseSensitive()\t= " << meta->isCaseSensitive(column);
    std::cout << "/" << ps_meta->isCaseSensitive(column) << std::endl;
    if (meta->isCaseSensitive(column) != ps_meta->isCaseSensitive(column))
      throw std::runtime_error("isCaseSensitive different for PS and non-PS");

    std::cout << "#\t\t meta->isCurrency()\t\t= " << meta->isCurrency(column);
    std::cout << "/" << ps_meta->isCurrency(column) << std::endl;
    if (meta->isCurrency(column) != ps_meta->isCurrency(column))
      throw std::runtime_error("isCurrency different for PS and non-PS");

    std::cout << "#\t\t meta->isDefinitelyWritable()\t= " << meta->isDefinitelyWritable(column);
    std::cout << "/" << ps_meta->isDefinitelyWritable(column) << std::endl;
    if (meta->isDefinitelyWritable(column) != ps_meta->isDefinitelyWritable(column))
      throw std::runtime_error("isDefinitelyWritable different for PS and non-PS");

    std::cout << "#\t\t meta->isNullable()\t\t= " << meta->isNullable(column);
    std::cout << "/" << ps_meta->isNullable(column) << std::endl;
    if (meta->isNullable(column) != ps_meta->isNullable(column))
      throw std::runtime_error("isNullable different for PS and non-PS");

    std::cout << "#\t\t meta->isReadOnly()\t\t= " << meta->isReadOnly(column);
    std::cout << "/" << ps_meta->isReadOnly(column) << std::endl;
    if (meta->isReadOnly(column) != ps_meta->isReadOnly(column))
      throw std::runtime_error("isReadOnly different for PS and non-PS");

    std::cout << "#\t\t meta->isSearchable()\t\t= " << meta->isSearchable(column);
    std::cout << "/" << ps_meta->isSearchable(column) << std::endl;
    if (meta->isSearchable(column) != ps_meta->isSearchable(column))
      throw std::runtime_error("isSearchable different for PS and non-PS");

    std::cout << "#\t\t meta->isSigned()\t\t= " << meta->isSigned(column);
    std::cout << "/" << ps_meta->isSigned(column) << std::endl;
    if (meta->isSigned(column) != ps_meta->isSigned(column))
      throw std::runtime_error("isSigned different for PS and non-PS");

    std::cout << "#\t\t meta->isWritable()\t\t= " << meta->isWritable(column);
    std::cout << "/" << ps_meta->isWritable(column) << std::endl;
    if (meta->isWritable(column) != ps_meta->isWritable(column))
      throw std::runtime_error("isWritable different for PS and non-PS");
  }

  try {
    res->close();
    meta = res->getMetaData();
  } catch (sql::SQLException &e) {
    std::cout << "#\t Excepted exception (I) when trying to get meta data after close:" << std::endl;
    std::cout << "#\t " << e.what() << std::endl;
  }

  try {
    ps_res->close();
    ps_meta = ps_res->getMetaData();
  } catch (sql::SQLException &e) {
    std::cout << "#\t Excepted exception (II) when trying to get meta data after close:" << std::endl;
    std::cout << "#\t " << e.what() << std::endl;
  }
}
