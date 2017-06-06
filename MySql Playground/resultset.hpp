#pragma once

static void validateResultSet(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *min, struct _test_data *max);

static void validateRow(boost::scoped_ptr< sql::ResultSet > & res, struct _test_data *exp);

int main_resultset(int argc, const char **argv);
