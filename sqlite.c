#include "multimon.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sqlite3.h>


//TODO Error handling

int store_message (int sql_address, int sql_function, char *sql_message, char *sql_baud)
{
	int error = 0;
	int timestamp;
	sqlite3 *message_db;
	const char create_table[] = "CREATE TABLE IF NOT EXISTS messages ("
		"address INTEGER, " 
		"function INTEGER, "
		"message TEXT, "
		"baud TEXT, "
		"timestamp INTEGER"
		")";
	const char insert_message[] = "INSERT INTO messages"
		"(address, function, message, baud, timestamp)"
		"VALUES (?,?,?,?,?)";
	sqlite3_stmt *insert_stmt = NULL;
	
	//Open database, will create if not existing
	error = sqlite3_open  (pocsag_database, &message_db);
	if (error)
	{
		printf ("There was a problem creating/opening the message database \n");
		return (1);
	}

	//Create message table
	error = sqlite3_exec (message_db, create_table, 0 ,0 ,0);
	error = sqlite3_prepare_v2 (message_db, insert_message, -1, &insert_stmt, NULL);
	//Read Unix time
	timestamp = time(NULL);
	// Bind values
	error = sqlite3_bind_int (insert_stmt, 1, sql_address);
	error = sqlite3_bind_int (insert_stmt, 2, sql_function);
	error = sqlite3_bind_text (insert_stmt, 3, sql_message, strlen(sql_message), NULL);
	error = sqlite3_bind_text (insert_stmt, 4, sql_baud, strlen(sql_baud), NULL);
	error = sqlite3_bind_int64 (insert_stmt, 5, timestamp);
	//Store
	error = sqlite3_step (insert_stmt);
	//Close DB
	error = sqlite3_finalize (insert_stmt);
	sqlite3_close (message_db);
	return (0);
}
