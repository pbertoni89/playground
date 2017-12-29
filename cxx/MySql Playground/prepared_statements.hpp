#pragma once

bool prepare_execute(boost::scoped_ptr< sql::Connection > & con, const char *sql);
sql::Statement* emulate_prepare_execute(boost::scoped_ptr< sql::Connection > & con, const char *sql);

int main_prepared_statement(int argc, const char **argv);
