#pragma once

int main_exceptions(int argc, const char **argv);

bool prepare_execute(sql::Connection *con, const char *sql);

sql::Statement* emulate_prepare_execute(sql::Connection *con, const char *sql);
