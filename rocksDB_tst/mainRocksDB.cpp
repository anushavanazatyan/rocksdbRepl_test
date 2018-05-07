//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <assert.h>
#include "rocksdb/db.h"
//#include "rocksdb/c.h"
//#include <iostream>
/////////////
//#include "db/write_batch_internal.h"
#//include "rocksdb/db.h"
//#include "rocksdb/types.h"
//#include "util/gflags_compat.h"
//#include "util/testutil.h"

////////////
//#include <unistd.h>  // sysconf() - get CPU count

//const char DBPath[] = "/tmp/rocksdb_simple_example";
//const char DBBackupPath[] = "/tmp/rocksdb_simple_example_backup";

int main(int argc, char **argv) {
//	rocksdb::DB
	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;
//	rocksdb::SequenceNumber secNum = db->GetLatestSequenceNumber();
//	rocksdb::unique_ptr <rocksdb::TransactionLogIterator> iter;
//	rocksdb::Status status = db->GetUpdatesSince(secNum, &iter);

	rocksdb::Status status =
		rocksdb::DB::Open(options, "../tmp/testdb", &db);
	assert(status.ok());

	std::string key2 = "key";
	std::string value = "random text ";
	rocksdb::Status s;
	for (int i = 0; i< 100; i++)
	{
		key2 = key2 + std::to_string(i);
		value = value + std::to_string(i);
		s = db->Get(rocksdb::ReadOptions(), key2, &value);
		if (!s.ok()) s = db->Put(rocksdb::WriteOptions(), key2, value);

	}
	
	


//	rocksdb::DataPumpThread()
#if 0
	rocksdb_t *db_t;
	rocksdb_backup_engine_t *be;
	rocksdb_options_t *options_t = rocksdb_options_create();


// Optimize RocksDB. This is the easiest way to
// get RocksDB to perform well
//	long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
//	rocksdb_options_increase_parallelism(options, (int)(cpus));
//	rocksdb_options_optimize_level_style_compaction(options, 0);
	// create the DB if it's not already present
	rocksdb_options_set_create_if_missing(options_t, 1);
	
	// open DB
	char *err = NULL;
	db_t = rocksdb_open(options_t, DBPath, &err);
	assert(!err);

	// open Backup Engine that we will use for backing up our database
	be = rocksdb_backup_engine_open(options_t, DBBackupPath, &err);
	assert(!err);
	
	// Put key-value
//	rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
	const char key[] = "key";
//	const char *value = "value";
//	rocksdb_put(db, writeoptions, key, strlen(key), value, strlen(value) + 1, &err);
//	assert(!err);
	// Get value
	rocksdb_readoptions_t *readoptions = rocksdb_readoptions_create();
	size_t len;
	char *returned_value =
		rocksdb_get(db_t, readoptions, key, strlen(key), &len, &err);
	assert(!err);
	assert(strcmp(returned_value, "value") == 0);
	free(returned_value);

	// create new backup in a directory specified by DBBackupPath
	rocksdb_backup_engine_create_new_backup(be, db_t, &err);
	assert(!err);

	rocksdb_close(db_t);

	// If something is wrong, you might want to restore data from last backup
	rocksdb_restore_options_t *restore_options = rocksdb_restore_options_create();
	rocksdb_backup_engine_restore_db_from_latest_backup(be, DBPath, DBPath,
		restore_options, &err);
	assert(!err);
	rocksdb_restore_options_destroy(restore_options);

	db_t = rocksdb_open(options_t, DBPath, &err);
	assert(!err);

	// cleanup
//	rocksdb_writeoptions_destroy(writeoptions);
	rocksdb_readoptions_destroy(readoptions);
	rocksdb_options_destroy(options_t);
	rocksdb_backup_engine_close(be);
	rocksdb_close(db_t);
#endif // if 0
	return 0;
}