#pragma once

/**
* Prints all meta data associated with an result set
*
*/
static void printResultSetMetaData(boost::scoped_ptr< sql::ResultSet > &res, boost::scoped_ptr< sql::ResultSet > &ps_res);

int main_resultset_meta(int argc, const char **argv);