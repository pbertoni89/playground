#pragma once


#define DYNLOAD_MYSQL_LIB "C:\\Users\\Xnext\\Desktop\\mysql-connector-c++-noinstall-1.1.9-winx64\\lib\\mysqlcppconn.lib"

/* *
* Basic example demonstrating how to load a different driver.
*
*/

static void validateResultSet(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *min, struct _test_data *max);

static void validateRow(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *exp);

int main_dynamic_load(int argc, const char **argv);
