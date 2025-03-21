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

/**
* Basic example of creating a stand alone program linked against Connector/C++
*
* This example is not integrated into the Connector/C++ build environment.
* You must run "make install" prior to following the build instructions
* given here.
*
* To compile the standalone example on Linux try something like:
*
* /usr/bin/c++
*   -o pthread
*   -I/usr/local/include/cppconn/
*   -Wl,-Bdynamic -lmysqlcppconn -pthread
*    examples/pthreads.cpp
*
* To run the example on Linux try something similar to:
*
*  LD_LIBRARY_PATH=/usr/local/lib/ ./pthread
*
* or:
*
*  LD_LIBRARY_PATH=/usr/local/lib/ ./pthread host user password database
*
*/


#ifndef _WIN32
/* usleep() */
#include <unistd.h>

/* Threading stuff */
#include <pthread.h>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS ""
#define EXAMPLE_DB "test"

struct st_worker_thread_param {
	sql::Driver *driver;
	sql::Connection *con;
};

using namespace std;

void* thread_one_action(void *arg);
int thread_finished = 0;

string url;
string user;
string pass;
string database;


/**
* Usage example for Driver, Connection, (simple) Statement, ResultSet
*/
int main_(int argc, const char **argv)
{
	sql::Driver *driver;
	boost::scoped_ptr< sql::Connection > con;

	url = (argc >= 2) ? argv[1] : EXAMPLE_HOST;
	user = (argc >= 3) ? argv[2] : EXAMPLE_USER;
	pass = (argc >= 4) ? argv[3] : EXAMPLE_PASS;
	database = (argc >= 5) ? argv[4] : EXAMPLE_DB;

	int status;
	pthread_t thread_one;

	std::cout << std::endl;
	std::cout << "Main thread: Connector/C++ phthreads program example..." << std::endl;
	std::cout << std::endl;

	try {
		driver = sql::mysql::get_driver_instance();

		/* Using the Driver to create a connection */
		con.reset(driver->connect(url, user, pass));
		con->setSchema(database);

		/* Worker thread */

		std::cout << "Main thread: creating thread 1..." << std::endl;
		/*
			A little bloat.
			We don't want global scoped_ptr objects. Therefore
			we wrap the object in an object. An alternative
			would have been to use global sql::Driver, sql::Connection
			objects [plain objects and no scoped_ptr] but then
			we'd have to add bloat for making sure we explicitly
			delete them, e.g. in case of an exception.
			It is not nice in either case. Let's use parameter struct.
		*/
		struct st_worker_thread_param *param = new st_worker_thread_param;
		param->driver = driver;
		param->con = con.get();

		status = pthread_create(&thread_one, NULL, thread_one_action, (void *)param);
		if (status != 0)
			throw std::runtime_error("Thread creation has failed");

		status = pthread_join(thread_one, NULL);
		if (status != 0)
			throw std::runtime_error("Joining thread has failed");

		while (thread_finished == 0) {
			/*
			The worker is using global resources which the main thread shall not
			free before the worker thread is done. For example, the worker
			thread is using the Connection object.
			*/
			std::cout << "Main thread: waiting for thread to finish fetching data..." << std::endl;
			usleep(300000);
		}

		delete param;

		std::cout << "Main thread: thread 1 has finished fetching data from MySQL..." << std::endl;

	} catch (sql::SQLException &e) {

		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return EXIT_FAILURE;

	} catch (std::runtime_error &e) {

		std::cout << "# ERR: std::runtime_error in " << __FILE__;
		std::cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what() << std::endl;
		std::cout << "not ok 1 - examples/connect.php" << std::endl;

		return EXIT_FAILURE;
	}

	std::cout << std::endl;
	std::cout << "Main thread: ... find more at http://www.mysql.com" << std::endl;
	std::cout << std::endl;
	return EXIT_SUCCESS;
}


void* thread_one_action(void *arg) {
	int status;
	boost::scoped_ptr< sql::Statement > stmt;
	boost::scoped_ptr< sql::ResultSet > res;

	struct st_worker_thread_param *handles = (struct st_worker_thread_param*) arg;

	/*
		NOTE:
		In every new thread you must call threadInit() *before*
		doing anything with the Connector. If you omit this
		step anything, including a crash, may happen.
	*/
	std::cout << std::endl;
	std::cout << "\tThread 1: driver->threadInit()" << std::endl;
	handles->driver->threadInit();

	std::cout << "\tThread 1: ... statement object created" << std::endl;
	stmt.reset(handles->con->createStatement());

	/*
		Sharing resultset among threads should
		be avoided. Its possible but requires further
		action from you.
	*/
	std::cout << "\tThread 1: ... running 'SELECT SLEEP(1), 'Welcome to Connector/C++' AS _message'" << std::endl;
	res.reset(stmt->executeQuery("SELECT SLEEP(1), 'Welcome to Connector/C++' AS _message"));

	std::cout << "\tThread 1: ... fetching result" << std::endl;
	while (res->next()) {
		std::cout << "\tThread 1: ... MySQL replies: " << res->getString("_message") << std::endl;
		std::cout << "\tThread 1: ... say it again, MySQL" << std::endl;
		std::cout << "\tThread 1: ... MySQL replies: " << res->getString(2) << std::endl;
	}

	std::cout << "\tThread 1: driver->threadEnd()" << std::endl;

	/*
		NOTE:
		You must call threadEnd() when the thread
		exits If you omit this step you get messages
		from the C Client Library like this:

		Error in my_thread_global_end(): 1 threads didn't exit

	*/
	handles->driver->threadEnd();
	std::cout << std::endl;

	thread_finished = 1;

	return NULL;
}
#endif
