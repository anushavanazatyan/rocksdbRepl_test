#include "rocksdb/db.h"
//#include <rocksdb/c.h>

#include <iostream>

using namespace rocksdb;
using namespace std;

int main()
{
#if 0
	cout << "test " << endl;
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
#endif

//	DB *db;
	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::SequenceNumber secNum = db->GetLatestSequenceNumber();
	rocksdb::unique_ptr<rocksdb::TransactionLogIterator> iter;
	rocksdb::Status status = db->GetUpdatesSince(secNum, &iter);


}

struct ReplicationThread {
	std::atomic<bool> stop;
	DB* db;
	volatile size_t no_read;
};

static void ReplicationThreadBody(void* arg) {
	ReplicationThread* t = reinterpret_cast<ReplicationThread*>(arg);
	DB* db = t->db;
	unique_ptr<TransactionLogIterator> iter;
	SequenceNumber currentSeqNum = db->GetLatestSequenceNumber();//1;
	while (!t->stop.load(std::memory_order_acquire)) {
		iter.reset();
		Status s;
		while (!db->GetUpdatesSince(currentSeqNum, &iter).ok()) {
			if (t->stop.load(std::memory_order_acquire)) {
				return;
			}
		}
		fprintf(stderr, "Refreshing iterator\n");
		for (; iter->Valid(); iter->Next(), t->no_read++, currentSeqNum++) {
			BatchResult res = iter->GetBatch();
			if (res.sequence != currentSeqNum) {
				fprintf(stderr, "Missed a seq no. b/w %ld and %ld\n",
					(long)currentSeqNum, (long)res.sequence);
				exit(1);
			}
		}
	}
}